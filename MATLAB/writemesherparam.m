function [] = writemesherparam(fname,P,meshname)
%WRITEMESHERPARAM write mesher parameter structure to file
%   fname - filename to write parameter file to
%   P - parameter structure from getmesherparam
%   [meshname] - optional name for comment in header
%
% Currently THERE IS NO CHECKING OF VALID PARAMETERS

fid=fopen(fname,'w+');

fprintf(fid,'# Mesher parameter file %s',datestr(now));

if exist('meshname','var') == 1 && ~isempty(meshname)
    fprintf(fid,' designed for mesh %s',meshname);
end
fprintf(fid,'\r\n');

%planar refinement
fprintf(fid,'#Planar refinment (1 - yes, 0 -no)\r\n');
fprintf(fid,'planar_refinement = %d\r\n',P.planar.planar_refinement);
fprintf(fid,'height = %.3f\r\n',P.planar.height);
fprintf(fid,'planar_direction_xyz = %.3f\r\n',P.planar.planar_direction_xyz);
fprintf(fid,'elements_with_fine_sizing_field_percentage = %.3f\r\n',P.planar.elements_with_fine_sizing_field_percentage);
fprintf(fid,'\r\n\r\n');

%spherical refinement
fprintf(fid,'#Spherical refinment (1 - yes, 0 - no)\r\n');
fprintf(fid,'#Distances in mm\r\n');
fprintf(fid,'sphere_refinement = %d\r\n',P.spherical.sphere_refinement);
fprintf(fid,'sphere_radius = %.3f\r\n',P.spherical.sphere_radius);
fprintf(fid,'sphere_centre_x = %.3f\r\n',P.spherical.sphere_centre_x);
fprintf(fid,'sphere_centre_y = %.3f\r\n',P.spherical.sphere_centre_y);
fprintf(fid,'sphere_centre_z = %.3f\r\n',P.spherical.sphere_centre_z);
fprintf(fid,'sphere_cell_size = %.3f\r\n',P.spherical.sphere_cell_size);
fprintf(fid,'\r\n\r\n');

%square refinement
fprintf(fid,'#Square refinement\r\n');
fprintf(fid,'square_refinement = %d\r\n',P.square.square_refinement);
fprintf(fid,'square_x_extent = %.3f\r\n',P.square.square_x_extent);
fprintf(fid,'square_y_extent = %.3f\r\n',P.square.square_y_extent);
fprintf(fid,'square_z_extent = %.3f\r\n',P.square.square_z_extent);
fprintf(fid,'square_centre_x = %.3f\r\n',P.square.square_centre_x);
fprintf(fid,'square_centre_y = %.3f\r\n',P.square.square_centre_y);
fprintf(fid,'square_centre_z = %.3f\r\n',P.square.square_centre_z);
fprintf(fid,'square_cell_size = %.3f\r\n',P.square.square_cell_size);
fprintf(fid,'\r\n\r\n');

% standard parameters
fprintf(fid,'#Standard refinement parameters\r\n');
fprintf(fid,'refine_electrodes = %d\r\n',P.refine_electrodes);
fprintf(fid,'pixel_scale_mm = %.3f\r\n',P.pixel_scale_mm);
fprintf(fid,'facet_angle_deg = %.3f\r\n',P.facet_angle_deg);
fprintf(fid,'facet_distance_mm = %.3f\r\n',P.facet_distance_mm);
fprintf(fid,'cell_radius_edge_ratio = %.3f\r\n',P.cell_radius_edge_ratio);
fprintf(fid,'cell_fine_size_mm = %.3f\r\n',P.cell_fine_size_mm);
fprintf(fid,'cell_coarse_size_mm = %.3f\r\n',P.cell_coarse_size_mm);
fprintf(fid,'electrode_radius_mm = %.3f\r\n',P.electrode_radius_mm);
fprintf(fid,'cell_size_electrodes_mm = %.3f\r\n',P.cell_size_electrodes_mm);
fprintf(fid,'\r\n\r\n');

%optimisation
fprintf(fid,'#Optimisation (1 - yes, 0 - no)\r\n');
fprintf(fid,'lloyd_opt = %d\r\n',P.opt.lloyd_opt);
fprintf(fid,'odt_opt = %d\r\n',P.opt.odt_opt);
fprintf(fid,'exude_opt = %d\r\n',P.opt.exude_opt);
fprintf(fid,'perturb_opt = %d\r\n',P.opt.perturb_opt);
fprintf(fid,'time_limit_sec = %d\r\n',P.opt.time_limit_sec);
fprintf(fid,'\r\n\r\n');

%saving
fprintf(fid,'#Save a vtu file of the mesh?\r\n');
fprintf(fid,'save_vtk = %d\r\n',P.save.save_vtk);
fprintf(fid,'#Save cell centres as csv?\r\n');
fprintf(fid,'save_cell_centres = %d\r\n',P.save.save_cell_centres);
fprintf(fid,'#Save nodes and tetra as csv?\r\n');
fprintf(fid,'save_nodes_tetra = %d\r\n',P.save.save_nodes_tetra);
fprintf(fid,'\r\n\r\n');

%deformation
fprintf(fid,'#Deform the mesh?\r\n');
fprintf(fid,'do_deformation = %d\r\n',P.deform.do_deformation);
fprintf(fid,'deform_x = %.3f\r\n',P.deform.deform_x);
fprintf(fid,'deform_y = %.3f\r\n',P.deform.deform_y);
fprintf(fid,'deform_z = %.3f\r\n',P.deform.deform_z);
fprintf(fid,'num_deformations = %d\r\n',P.deform.num_deformations);
fprintf(fid,'min_stretch_distance = %.3f\r\n',P.deform.min_stretch_distance);
fprintf(fid,'max_stretch_distance = %.3f\r\n',P.deform.max_stretch_distance);
fprintf(fid,'\r\n\r\n');

%move electrodes
fprintf(fid,'#Move electrodes to the closest facet in the mesh? Useful is electrodes not exactly on the mesh\r\n#Will move electrode to the nearest facet with tissue type equal to outermost_tissue\r\n');
fprintf(fid,'move_electrodes = %d\r\n',P.move.move_electrodes);
fprintf(fid,'outermost_tissue = %d\r\n',P.move.outermost_tissue);

end

