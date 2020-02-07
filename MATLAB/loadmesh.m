function [Mesh] = loadmesh(fname)
%READMESH reads mesh from tetra and vert csv files into Mesh structure
%   Assumes files all in same directory, with same basename fname:
%   fname.parameters (meshing parameters and ground node position)
%   fname.electrode (Electrode locations in mesh coordinates)
%   fname.dgf (not read)
%   fname_tetra.csv
%   fname_verticies.csv (save_nodes_tetra =1 must be set in Mesher Parameter file) 

disp(['Loading mesh : ' fname]);

electrode_positions=csvread([fname '.electrodes']);

verticies=csvread([fname '_vertices.csv']);
tetra=csvread([fname '_tetra.csv']);

% read parameter file
pfid=fopen([fname '.parameters']);

t=textscan(pfid,'%s','Delimiter','\n');

fclose(pfid);

% find the ground position 
t=t{1};
t=cellstr(t);

% remove comments
t=t(~startsWith(t,'#'));

% find ground node position
x_idx=contains(t,'groundposition.x:');
y_idx=contains(t,'groundposition.y:');
z_idx=contains(t,'groundposition.z:');

gndx=sscanf(t{x_idx}(19:end),'%g');
gndy=sscanf(t{y_idx}(19:end),'%g');
gndz=sscanf(t{z_idx}(19:end),'%g');

gnd_pos = [gndx, gndy, gndz];

Mesh.Tetra=tetra(:,1:4);
Mesh.Nodes=verticies(:,1:3);
Mesh.mat_ref=int8(tetra(:,5));
Mesh.elec_pos=electrode_positions;
Mesh.gnd_pos=gnd_pos;

end

