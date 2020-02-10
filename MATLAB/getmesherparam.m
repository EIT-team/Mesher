function [P] = getmesherparam()
%GETMESHERPARAM Creates default structure of Mesher parameters
%  Description of the parameters here 
%  https://github.com/EIT-team/Mesher/wiki/Mesher-input-parameters

% Refinements
P.refine.electrodes = 0;
P.refine.depth = 0;
P.refine.planar = 0;
P.refine.cuboid = 0;
P.refine.sphere = 0;

% Standard refinement parameters
P.facet_angle_deg = 30;
P.facet_distance_mm = 5;
P.cell_radius_edge_ratio = 3;
P.cell_coarse_size_mm = 5;

% Electrode refinement
P.electrode_radius_mm = 1;
P.cell_size_electrodes_mm = 5;

% Depth refinement
P.cell_fine_size_mm = 5;
P.elements_with_fine_sizing_field_percentage = 50;

%Planar refinment (1 - yes, 0 -no)
P.planar.height = 1;
P.planar.planar_direction_xyz = 3; %x=1,y=2,z=3

% Spherical refinement (1 - yes, 0 - no)
% Distances in mm
P.sphere.sphere_radius = 2;
P.sphere.sphere_centre_x = 13.5;
P.sphere.sphere_centre_y = 13.2;
P.sphere.sphere_centre_z = 17.9;
P.sphere.sphere_cell_size = 0.5;

% cuboid refinement
P.cuboid.cuboid_x_extent = 4;
P.cuboid.cuboid_y_extent = 6;
P.cuboid.cuboid_z_extent = 4;
P.cuboid.cuboid_centre_x = 13.9;
P.cuboid.cuboid_centre_y = 13.2;
P.cuboid.cuboid_centre_z = 17.9;
P.cuboid.cuboid_cell_size = 0.09;

% Optimisation (1 - yes, 0 - no)
P.opt.lloyd_opt = 0;
P.opt.odt_opt = 0;
P.opt.exude_opt = 0;
P.opt.perturb_opt = 0;
P.opt.time_limit_sec = 300;

% Save a vtu file of the mesh?
P.save.save_vtk = 1;

% Save cell centres as csv?
P.save.save_cell_centres = 0;

% Save nodes and tetra as csv?
P.save.save_nodes_tetra = 1;

% Deform the mesh?
P.deform.do_deformation = 0;
P.deform.deform_x = 0;
P.deform.deform_y = 0;
P.deform.deform_z = 0;
P.deform.num_deformations = 5;
P.deform.min_stretch_distance = 5;
P.deform.max_stretch_distance = 50;

% Move electrodes to the closest facet in the mesh? Useful is electrodes not exactly on the mesh
% Will move electrode to the nearest facet with tissue type equal to outermost_tissue
P.move.move_electrodes = 0;
P.move.outermost_tissue = 1;

end

