
Mesh=loadmesh('output/NNPEITSmesh');

cond_scalp=0.2;
cond_skull=0.03;

% create conductivity vector
cond= zeros( size(Mesh.Tetra,1) ,1);

cond(Mesh.mat_ref == 1) = cond_scalp;
cond(Mesh.mat_ref == 2) = cond_skull;


dune_exporter(Mesh.Nodes,Mesh.Tetra,cond,'output2/','NNP2',Mesh.elec_pos,Mesh.gnd_pos);
