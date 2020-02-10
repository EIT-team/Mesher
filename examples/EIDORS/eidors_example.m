%% EIDORS example

% Make sure you have added Mesher/MATLAB to the path, and ran EIDORS
% startup.m


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
runmesher('../neonatescalp/NNscalp.inr','../neonatescalp/NNscalp_elecINRpos.txt',...
    'NNEIDORS_param.txt','output/','NNEIDORS')
%% View Mesh and check electrodes

% Check electrodes are aligned correctly, and output is sensible before
% running computationally expensive output

Mesh=loadmesh('output/NNEIDORS');

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


%% Load into EIDORS

%remember to run eidors startup

%based on mk_fmdl_from_nodes
MDL=eidors_obj('fwd_model','test');

MDL.nodes = Mesh.Nodes;
MDL.elems = Mesh.Tetra;

[srf, idx] = find_boundary(Mesh.Tetra);

MDL.boundary = srf;

%find ground node
gnd_dist=sqrt(sum((Mesh.Nodes - Mesh.gnd_pos).^2,2));
[~, gnd_node] = min(gnd_dist);
MDL.gnd_node = gnd_node;

% Electrodes

% find nodes on the suface within electrode radius
z_contact = 100;
elec_radius = .008; %in meters

srf_idx=unique(srf(:));
srf_nodes=Mesh.Nodes(srf_idx,:);

for iElec = 1:size(Mesh.elec_pos,1)
    electrodes(iElec).z_contact= z_contact;
    edist = sqrt(sum((srf_nodes - Mesh.elec_pos(iElec,:)).^2,2));
    enodes = srf_idx(edist <= elec_radius);
    
    curNodes=enodes';
    curNodes = unique(curNodes);
    
    % only use nodes which can be used in triangulation as this confuses
    % find_electrode_bdy.m, as it thinks any unused node is a point electrode.
    % This is adapted from find_electrode_bdy
    
    % count how many times each node is used in the surface
    bdy_els = zeros(size(srf,1),1);
    for nd= curNodes(:)'
        bdy_els = bdy_els + any(srf==nd,2);
    end
    % Nodes used three times are part of a triangulation
    ffb = find(bdy_els == size(srf,2));
    % only use nodes which appear 3 times 
    curNodes=intersect(curNodes,unique(srf(ffb,:)));
    electrodes(iElec).nodes = curNodes;
end

MDL.electrode =     electrodes;
MDL.solve=          @fwd_solve_1st_order;
MDL.jacobian=       @jacobian_adjoint;
MDL.system_mat=     @system_mat_1st_order;
MDL.normalize_measurements = 0;

figure
show_fem(MDL)
title('Mesher output in EIDORS');
saveas(gcf,'figures/EIDORS_FEM.png');
%% Forward model settings

% Convert protocol file to EIDORS stim patterns

Amp=240e-6;
N_elec=size(Mesh.elec_pos,1); 

prt=dlmread('NN2016_Prt_full.txt');

[stim]= stim_meas_list( prt,N_elec,Amp);

MDL.stimulation=stim;

%% Check FWD is ok
if valid_fwd_model(MDL)
    disp('Forward model is ok!');
end

%% Add conductivity and solve

cond_scalp=0.2;
cond_skull=0.03;

% create conductivity vector
cond= zeros( size(MDL.elems,1) ,1);

cond(Mesh.mat_ref == 1) = cond_scalp;
cond(Mesh.mat_ref == 2) = cond_skull;

img = mk_image(MDL,cond);

% solve fwd for this conductivity 
data=fwd_solve( img);

figure
plot(data.meas)
title('Simulated voltaged from EIDORS')
xlabel('Measurement');
ylabel('Voltage (V)');
ylim([-0.05 0.05])
% saveas(gcf,'figures/EIDORS_Volts.png')
