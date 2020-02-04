%% EIDORS example

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

inv_mdl= mk_common_model('n3r2',[16,2]);
show_fem(inv_mdl.fwd_model)
 
valid_fwd_model(inv_mdl.fwd_model)
%% 

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

%% do electrodes 

% find nodes on the suface within electrode radius 
z_contact = 200;
elec_radius = .006; %in meters

srf_idx=unique(srf(:));

srf_nodes=Mesh.Nodes(srf_idx,:);

for iElec = 1:size(Mesh.elec_pos,1)
    electrodes(iElec).z_contact= z_contact;
    edist = sqrt(sum((srf_nodes - Mesh.elec_pos(iElec,:)).^2,2));
    enodes = srf_idx(edist <= elec_radius);
    electrodes(iElec).nodes = enodes';
end

MDL.electrode =     electrodes;
MDL.solve=          'eidors_default';
MDL.jacobian=       'eidors_default';
MDL.system_mat=     'eidors_default';
MDL.normalize_measurements = 0;

%% Stimulation pattern

[stim, meas_sel] = mk_stim_patterns( 32,1,'{op}','{op}');

MDL.stimulation=stim;

%%
show_fem(MDL)
valid_fwd_model(MDL)





