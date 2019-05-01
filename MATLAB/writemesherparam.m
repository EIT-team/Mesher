function [outputArg1,outputArg2] = writemesherparam(fname,P,meshname)
%WRITEMESHERPARAM write mesher parameter structure to file
%   Detailed explanation goes here

fid=fopen(fname,'w+');

fprintf(fid,'# Mesher parameter file %s',datestr(now));

if exist('meshname','var') == 1 && ~isempty(meshname)
fprintf(fid,' designed for mesh %s',meshname);
end
fprintf(fid,'\n\n');

%planar refinement
fprintf(fid,'#Planar refinment (1 - yes, 0 -no)\n');
fprintf(fid,'planar_refinement = %d\n',P.planar.planar_refinement);
fprintf(fid,'height = %.3f\n',P.planar.height);
fprintf(fid,'planar_xyz = %.3f\n',P.planar.planar_xyz);
fprintf(fid,'elements_with_fine_sizing_field_percentage = %.3f\n',P.planar.elements_with_fine_sizing_field_percentage);
fprintf(fid,'\n\n');

%spherical refinement
fprintf(fid,'#Spherical refinment (1 - yes, 0 - no)\n');
fprintf(fid,'#Distances in mm\n');
fprintf(fid,'sphere_refinement = %d\n',P.spherical.sphere_refinement);
fprintf(fid,'sphere_radius = %.3f\n',P.spherical.sphere_radius);
fprintf(fid,'sphere_centre_x = %.3f\n',P.spherical.sphere_centre_x);
fprintf(fid,'sphere_centre_y = %.3f\n',P.spherical.sphere_centre_y);
fprintf(fid,'sphere_centre_z = %.3f\n',P.spherical.sphere_centre_z);
fprintf(fid,'sphere_cell_size = %.3f\n',P.spherical.sphere_cell_size);
fprintf(fid,'\n\n');

%square refinement
fprintf(fid,'#Square refinement\n');
fprintf(fid,'square_refinement = %d\n',P.square.square_refinement);
fprintf(fid,'square_x_extent = %.3f\n',P.square.square_x_extent);
fprintf(fid,'square_y_extent = %.3f\n',P.square.square_y_extent);
fprintf(fid,'square_z_extent = %.3f\n',P.square.square_z_extent);
fprintf(fid,'square_centre_x = %.3f\n',P.square.square_centre_x);
fprintf(fid,'square_centre_y = %.3f\n',P.square.square_centre_y);
fprintf(fid,'square_centre_z = %.3f\n',P.square.square_centre_z);
fprintf(fid,'square_cell_size = %.3f\n',P.square.square_cell_size);
fprintf(fid,'\n\n');

% standard parameters 
fprintf(fid,'#Standard refinement parameters\n');
fprintf(fid,'refine_electrodes = %d\n',P.refine_electrodes);
fprintf(fid,'pixel_scale_mm = %.3f\n',P.pixel_scale_mm);
fprintf(fid,'facet_angle_deg = %.3f\n',P.facet_angle_deg);
fprintf(fid,'facet_distance_mm = %.3f\n',P.facet_distance_mm);
fprintf(fid,'cell_radius_edge_ratio = %.3f\n',P.cell_radius_edge_ratio);
fprintf(fid,'cell_fine_size_mm = %.3f\n',P.cell_fine_size_mm);
fprintf(fid,'cell_coarse_size_mm = %.3f\n',P.cell_coarse_size_mm);
fprintf(fid,'electrode_radius_mm = %.3f\n',P.electrode_radius_mm);
fprintf(fid,'cell_size_electrodes_mm = %.3f\n',P.cell_size_electrodes_mm);
fprintf(fid,'\n\n');

%optimisation
fprintf(fid,'#Optimisation (1 - yes, 0 - no)\n');
fprintf(fid,'lloyd_opt = %d\n',P.opt.lloyd_opt);
fprintf(fid,'odt_opt = %d\n',P.opt.odt_opt);
fprintf(fid,'exude_opt = %d\n',P.opt.exude_opt);
fprintf(fid,'perturb_opt = %d\n',P.opt.perturb_opt);
fprintf(fid,'time_limit_sec = %d\n',P.opt.time_limit_sec);
fprintf(fid,'\n\n');

%saving
fprintf(fid,'#Save a vtu file of the mesh?\n');
fprintf(fid,'save_vtk = %d\n',P.save.save_vtk);
fprintf(fid,'#Save cell centres as csv?\n');
fprintf(fid,'save_cell_centres = %d\n',P.save.save_cell_centres);
fprintf(fid,'#Save nodes and tetra as csv?\n');
fprintf(fid,'save_nodes_tetra = %d\n',P.save.save_nodes_tetra);
fprintf(fid,'\n\n');

%deformation
fprintf(fid,'#Deform the mesh?\n');
fprintf(fid,'do_deformation = %d\n',P.deform.do_deformation);
fprintf(fid,'deform_x = %.3f\n',P.deform.deform_x);
fprintf(fid,'deform_y = %.3f\n',P.deform.deform_y);
fprintf(fid,'deform_z = %.3f\n',P.deform.deform_z);
fprintf(fid,'num_deformations = %d\n',P.deform.num_deformations);
fprintf(fid,'min_stretch_distance = %.3f\n',P.deform.min_stretch_distance);
fprintf(fid,'max_stretch_distance = %.3f\n',P.deform.max_stretch_distance);
fprintf(fid,'\n\n');

%move electrodes
fprintf(fid,'#Move electrodes to the closest facet in the mesh? Useful is electrodes not exactly on the mesh\n#Will move electrode to the nearest facet with tissue type equal to outermost_tissue\n');
fprintf(fid,'move_electrodes = %d\n',P.move.move_electrodes);
fprintf(fid,'outermost_tissue = %d\n',P.move.outermost_tissue);

end

