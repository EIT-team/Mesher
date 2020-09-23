%% EIDORS example

% Make sure you have added Mesher/MATLAB to the path, and ran EIDORS
% startup.m - Tested with EIDORS 3.10

% Mesher outputs ground node and electrode *coordinates* but EIDORS
% requires node references. So this code finds the required nodes.
% EIDORS also requires the measurement protocol in a stim structure, so
% this is converted from the txt file that PEITS uses.

%% Run mesher
% Settings for a lower resolution (~100k elem) output mesh for demonstration

% Get the parameter structure
P=getmesherparam;

P.facet_distance_mm=2;
P.cell_coarse_size_mm=4;

P.refine.electrodes = 1;
P.electrode_radius_mm=3;
P.cell_size_electrodes_mm=2;

% Turn off all optimisations
P.opt.exude_opt=0;
P.opt.lloyd_opt=0;
P.opt.odt_opt=0;
P.opt.perturb_opt=0;

% save the output to csv to load into matlab
P.save.save_nodes_tetra=1;
% save VTK to view it paraview
P.save.save_vtk=0;

% Move the electrode positions to ensure they are placed on the surface
P.move.move_electrodes=1;
P.move.outermost_tissue=1;

% write parameter file
writemesherparam('NNEIDORS_param.txt',P);

% Run mesher
runmesher('../../neonatescalp/NNscalp.inr','../../neonatescalp/NNscalp_elecINRpos.txt',...
    'NNEIDORS_param.txt','output/','NNEIDORS')
%% View Mesh and check electrodes

% Check electrodes are aligned correctly, and output is sensible before
% running computationally expensive output

% load Mesher output - either existing in the repo or the one just made
Mesh=loadmesh('output/NNEIDORS');
% load('NNEIDORS_ex'); % example output if MESHER not run

figure
hold on;
trep = triangulation(Mesh.Tetra, Mesh.Nodes);
[Triangle_Boundary, Nodes_Boundary] = freeBoundary(trep);
h= trisurf(Triangle_Boundary, Nodes_Boundary(:,1), Nodes_Boundary(:,2), Nodes_Boundary(:,3));

set(h,'EdgeColor',[0.3,0.3,0.3],'FaceColor','w','FaceAlpha',1);
daspect([1,1,1]);

plot3(Mesh.elec_pos(:,1),Mesh.elec_pos(:,2),Mesh.elec_pos(:,3),'.','Markersize',40);

hold off
view(3)
title('Neonate Mesh Low Res')

% Here you would now want to rerun the mesher with optimisations on, and
% higher density if you wish especially around the electrodes

%% Load into EIDORS

% remember to run eidors startup!

% this is largely based on mk_fmdl_from_nodes

% create blank fwd object
MDL=eidors_obj('fwd_model','NeonateHead');

% specify nodes and triangulation
MDL.nodes = Mesh.Nodes;
MDL.elems = Mesh.Tetra;

% specify boundary elements
[srf, idx] = find_boundary(Mesh.Tetra);

MDL.boundary = srf;

% find ground node - Mesher saves XYZ coordinates so find closest node
gnd_dist=sqrt(sum((Mesh.Nodes - Mesh.gnd_pos).^2,2));
[~, gnd_node] = min(gnd_dist);
MDL.gnd_node = gnd_node;

%% Electrodes
% Mesher saves electrodes as position, whereras EIDORS requires the nodes
% and contact impedance to be given within a structure. So here we find 
% nodes on the suface within electrode radius

z_contact = 20e-3; % ohms/m2 
elec_radius = .008; %in meters

% unique surface node references
srf_tri=unique(srf(:));
srf_nodes=Mesh.Nodes(srf_tri,:);

% loop through each electrode
for iElec = 1:size(Mesh.elec_pos,1)
    
    %assign conductivity to structure
    electrodes(iElec).z_contact= z_contact;
    
    % find surface nodes within electrode radius
    edist = sqrt(sum((srf_nodes - Mesh.elec_pos(iElec,:)).^2,2));
    enodes = srf_tri(edist <= elec_radius);
    
    curNodes=enodes';
    curNodes = unique(curNodes);
    
    % only use nodes which can be used in triangulation as this confuses
    % find_electrode_bdy.m, as it thinks any unused node is a point electrode.
    % This is adapted from find_electrode_bdy to find surface nodes that
    % form a complete surface triangle (in srf) i.e. form a complete triangulation
    
    % count how many times each node ref is used in the surface
    % triangulation
    boundary_els = zeros(size(srf,1),1);
    for nd= curNodes(:)'
        boundary_els = boundary_els + any(srf==nd,2);
    end
    % Complete surface triangles are ones where each node is within radius
    % i.e. curNodes contains each column in srf
    elec_tri = find(boundary_els == size(srf,2));
    % remove references to nodes that were not part of a full triangle
    curNodes=intersect(curNodes,unique(srf(elec_tri,:)));
    electrodes(iElec).nodes = curNodes;
end

% add electrode strucutre to model struct
MDL.electrode =     electrodes;

figure
show_fem(MDL)
title('Mesher output in EIDORS');
% saveas(gcf,'figures/EIDORS_FEM.png');
%% Forward model settings

% set some default solver parameters
MDL.solve=          @fwd_solve_1st_order;
MDL.jacobian=       @jacobian_adjoint;
MDL.system_mat=     @system_mat_1st_order;
MDL.normalize_measurements = 0;

% Convert protocol file to EIDORS stim patterns

Amp=240e-6; % Current amplitude in Amps

N_elec=size(Mesh.elec_pos,1); 

% PEITS and Supersolver use text file with following columns:
% CS+ CS- V+ V-
% 16,2,1,33
% 16,2,2,33
% 16,2,3,33
% ...
% i.e. inject between 16 and 2, measure 1:32 referenced to 33 etc.
prt=dlmread('NN2016_Prt_full.txt');

% create stimulation structure
[stim]= stim_meas_list( prt,N_elec,Amp);

MDL.stimulation=stim;

%% Check FWD is ok
if valid_fwd_model(MDL)
    disp('Forward model is ok!');
end

%% Add conductivity 

% Mesher specifies layer index for each element within mat_ref. This mesh
% has two layers : 1 for scalp and brain (saline), 2 for skull, as it is based on
% the head tank https://doi.org/10.1088/1361-6579/aa6586

cond_scalp=0.2;
cond_skull=0.03;

% create conductivity vector
cond= zeros( size(MDL.elems,1) ,1);

cond(Mesh.mat_ref == 1) = cond_scalp;
cond(Mesh.mat_ref == 2) = cond_skull;

% create image object using mesh and conductivity vector
img = mk_image(MDL,cond);
%% Solve

% solve forward model for this conductivity to get boundary voltages
data=fwd_solve(img);

figure
plot(data.meas)
title('Simulated voltaged from EIDORS')
xlabel('Measurement');
ylabel('Voltage (V)');
ylim([-0.15 0.15])
saveas(gcf,'figures/EIDORS_Volts.png')
