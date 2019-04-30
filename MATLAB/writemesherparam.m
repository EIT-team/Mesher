function [outputArg1,outputArg2] = writemesherparam(fname,P)
%WRITEMESHERPARAM write mesher parameter structure to file
%   Detailed explanation goes here

fid=fopen(fname,'w+');

fprintf(fid,'# Mesher parameter file %s\n',datestr(now));


fprintf(fid,'#Planar refinment (1 - yes, 0 -no)\n');
fprintf(fid,'planar_refinement = %d\n',P.planar.planar_refinement);

P.planar.height = 1;
P.planar.planar_xyz = 3;
P.planar.elements_with_fine_sizing_field_percentage = 50;

fprintf(fid,'#Spherical refinment (1 - yes, 0 - no)\n');
fprintf(fid,'#Distances in mm\n');
fprintf(fid,'sphere_refinement = %d\n',P.spherical.sphere_refinement);
P.spherical.sphere_radius = 2;
P.spherical.sphere_centre_x = 13.5;
P.spherical.sphere_centre_y = 13.2;
P.spherical.sphere_centre_z = 17.9;
P.spherical.sphere_cell_size = 0.5;

fprintf(fid,'#square refinement\n');
fprintf(fid,'square_refinement = %d\n',P.square.square_refinement);
P.square.square_x_extent = 4;
P.square.square_y_extent = 6;
P.square.square_z_extent = 4;
P.square.square_centre_x = 13.9;
P.square.square_centre_y = 13.2;
P.square.square_centre_z = 17.9;
P.square.square_cell_size = 0.09;

fprintf(fid,'#Standard refinement parameters\n');
fprintf(fid,'refine_electrodes = %d\n',P.refine_electrodes);
P.pixel_scale_mm = 0.025;

P.facet_angle_deg = 30;
P.facet_distance_mm = 5;
P.cell_radius_edge_ratio = 3;
P.cell_fine_size_mm = 5;
P.cell_coarse_size_mm = 5;
P.electrode_radius_mm = 1;
P.cell_size_electrodes_mm = 5;

fprintf(fid,'#Optimisation (1 - yes, 0 - no)\n');
fprintf(fid,'lloyd_opt = %d\n',P.opt.lloyd_opt);
P.opt.odt_opt = 0;
P.opt.exude_opt = 0;
P.opt.perturb_opt = 0;
P.opt.time_limit_sec = 300;

fprintf(fid,'#Save a vtu file of the mesh?\n');
P.save.save_vtk = 1;
fprintf(fid,'#Save cell centres as csv?\n');
P.save.save_cell_centres = 0;

fprintf(fid,'#Save nodes and tetra as csv?\n');
P.save.save_nodes_tetra = 1;

fprintf(fid,'#Deform the mesh?\n');

P.deform.do_deformation = 0;
P.deform.deform_x = 0;
P.deform.deform_y = 0;
P.deform.deform_z = 0;
P.deform.num_deformations = 5;
P.deform.min_stretch_distance = 5;
P.deform.max_stretch_distance = 50;

fprintf(fid,'#Move electrodes to the closest facet in the mesh? Useful is electrodes not exactly on the mesh\n#Will move electrode to the nearest facet with tissue type equal to outermost_tissue\n');
P.move.move_electrodes = 0;
P.move.outermost_tissue = 1;





end

