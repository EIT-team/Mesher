%% Brain example

% MRI segmentation was performed in Seg3D and saved to .mat file. In this
% case we have extracted the volume data from the file saved my Seg3D to
% save space:
%
% load('Seg3D_Out');
% V=scirunnrrd.data;
% save('Brain_seg.mat','V');
%
% Requires iso2mesh for plotting with binsurface only.

% This segmentation was resampled to give a uniform 
load('Brain_seg.mat');
pixel_scale=8;
vol_res=1/pixel_scale;
xyz_res=[vol_res,vol_res,vol_res];

elec_pos_sc=dlmread('Brain_elec.txt');

%plot if we have iso2mesh
if ~isempty(which('plotmesh'))
    
    % extract surface of each layer for plotting
    % binsurface needs [0 1] only, and can only do 1 surface at a time
    V1=V;
    V1(V1==2)=0;
    V2=V;
    V2(V2==1)=0;
    V2(V2==2)=1;
    
    [node1,elem1]=binsurface(V1,4);
    [node2,elem2]=binsurface(V2,4);
    
    figure
    hold on
    patch('Vertices',node1,'faces',elem1,'FaceColor','none','EdgeAlpha',1,'EdgeColor','r');
    patch('Vertices',node2,'faces',elem2,'FaceColor','none','EdgeAlpha',0.4,'EdgeColor',[0.4 0.4 0.4]);
    plot3(elec_pos_sc(:,1),elec_pos_sc(:,2),elec_pos_sc(:,3),'.','MarkerSize',30);
    legend('White Matter','Grey Matter','Electrodes');

    hold off
    daspect([1,1,1]);
    view(3);
    drawnow;
    title('Rat brain seg w/ white and grey matter layers')
    % the holes in the top of the grey matter layer are an artefact of the
    % binsurface output, they are not present in the inr (see fig below)
    
%     saveas(gcf,'figures/brain_seg.png');
    
end


figure
imagesc(squeeze(V(:,:,100))');
daspect([1,1,1]);
title('Slice through rat brain segmentation');
% saveas(gcf,'figures/brain_seg_slice.png');


% save the segmentation to inr for the mesher
saveinr_EIT(uint8(V),'Brain.inr',xyz_res);


%% Run the mesher

P=getmesherparam;
P.pixel_scale_mm=pixel_scale;
P.facet_distance_mm=.4;
P.cell_fine_size_mm=.5;
P.cell_coarse_size_mm=.6;
P.refine_electrodes = 1;
P.electrode_radius_mm=.3;
P.cell_size_electrodes_mm=.1;

% Turn off all optimisations
P.opt.exude_opt=0;
P.opt.lloyd_opt=0;
P.opt.odt_opt=0;
P.opt.perturb_opt=0;

% save the output to csv to load into matlab
P.save.save_nodes_tetra=1;
% save VTK to view it paraview
P.save.save_vtk=1;

% Move the electrode positions to ensure they are placed on the surface,
% the positions are not exact in the first place
P.move.move_electrodes=1;
P.move.outermost_tissue=2;

% write parameter file
writemesherparam('Brain_params.txt',P);

runmesher('Brain.inr','Brain_elec.txt','Brain_params.txt','output/','BrainMesh');


%% Plot mesh 

Mesh=loadmesh('output/BrainMesh');
figure
hold on;
trep = triangulation(Mesh.Tetra, Mesh.Nodes);
[Triangle_Boundary, Nodes_Boundary] = freeBoundary(trep);
h= trisurf(Triangle_Boundary, Nodes_Boundary(:,1), Nodes_Boundary(:,2), Nodes_Boundary(:,3));

set(h,'EdgeColor',[0.3,0.3,0.3],'FaceColor','w','FaceAlpha',1);
daspect([1,1,1]);

plot3(Mesh.elec_pos(:,1),Mesh.elec_pos(:,2),Mesh.elec_pos(:,3),'.','Markersize',30);

hold off
view(3)
title('Rat Brain Mesh Low Res')

% saveas(gcf,'figures/BrainMesh_LowRes.png')

%% Run at higher resolution

% create settings for a more realistic mesh 

P=getmesherparam;
P.pixel_scale_mm=pixel_scale;
P.facet_distance_mm=.4;
P.cell_fine_size_mm=.15; % Overall smaller elements
P.cell_coarse_size_mm=.2;  % Much higher lower bound of element size
P.refine_electrodes = 1;
P.electrode_radius_mm=.3;
P.cell_size_electrodes_mm=.05; % v high res around the electrodes

% Turn on all optimisations and give them a long time to run
P.opt.exude_opt=1;
P.opt.lloyd_opt=1;
P.opt.odt_opt=1;
P.opt.perturb_opt=1;
P.opt.time_limit_sec=86400;

% save the output to csv to load into matlab
P.save.save_nodes_tetra=0;
% save VTK to view it paraview
P.save.save_vtk=1;

% Move the electrode positions to ensure they are placed on the surface,
% the positions are not exact in the first place
P.move.move_electrodes=1;
P.move.outermost_tissue=2;

% write parameter file
writemesherparam('Brain_params_HR.txt',P);

runmesher('Brain.inr','Brain_elec.txt','Brain_params_HR.txt','output_HR/','BrainMeshHighRes');





