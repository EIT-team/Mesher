%% Neonate brain example

% convert the STLs to INR files. Here we are using pixel scale of 1, for
% example only. In reality we would want to use whatever captures the
% geometry best. A value of 5 was used in the papers using this phantom
pixel_scale=1;

% load electrode positions - these are in coordinates of the original STL
% and will be converted to the output INR coordinate system
elec_pos=dlmread('NNelecposorig.txt');
% run the conversion to INR. Plotting the figures to check the alignment of
% the electrodes and the quality of the output
[ full_mask,elec_pos_new_sc ] = stl2inr({'NNscalp.stl','NNskull_lowpoly.stl'},pixel_scale,elec_pos );

% saveas(1,'figures/NN_stl.png') % save figures for documentation
% saveas(2,'figures/NN_slice.png')
% saveas(3,'figures/NN_bin.png')

%% Run the Mesher - Low res

% Settings for a lower resolution (~100k elem) output mesh for demonstration

% Get the parameter structure
P=getmesherparam;
P.pixel_scale_mm=pixel_scale;
P.facet_distance_mm=2;
P.cell_fine_size_mm=3;
P.cell_coarse_size_mm=8;
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
P.save.save_vtk=1;

% Move the electrode positions to ensure they are placed on the surface
P.move.move_electrodes=1;
P.move.outermost_tissue=1;


% write parameter file
writemesherparam('NNscalp_param.txt',P);

% Run mesher
runmesher('NNscalp.inr','NNscalp_elecINRpos.txt',...
    'NNscalp_param.txt','output/','NNexample')
%% View Mesh and check electrodes

% Check electrodes are aligned correctly, and output is sensible before
% running computationally expensive output

Mesh=loadmesh('output/NNexample');
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

% saveas(gcf,'figures/NN_LowRes.png') 

%% Run Mesher again with realistic parameters


