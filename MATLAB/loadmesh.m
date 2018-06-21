function [Mesh] = loadmesh(fname)
%READMESH reads mesh from tetra and vert csv files into Mesh structure
%   Detailed explanation goes here
e=csvread([fname '.electrodes']);
vert=csvread([fname '_vertices.csv']);
tetra=csvread([fname '_tetra.csv']);
Mesh.Tetra=tetra(:,1:4);
Mesh.Nodes=vert(:,1:3);
Mesh.mat_ref=tetra(:,5);
Mesh.elec_pos=e;

end

