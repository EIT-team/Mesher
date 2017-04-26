mesh_name = '01000';
mesh_file = ['D:\Documents\Experimental Data\Mesh_deformation\Head meshes\01000\Mesh' mesh_name '.mat'];

Mesh = load(mesh_file, ['Mesh' mesh_name])

% Take the loaded variable and rename it as 'Mesh'
eval(['Mesh = Mesh.Mesh' mesh_name])

clear mesh_name mesh_file

%%
disp('Unique conductivity values:')
unique_sigma = unique(Mesh.Sigma);
num_sigma = size(unique_sigma,1);
disp(unique_sigma)

%%
New_mesh.Nodes = Mesh.Nodes;

for i = 1:num_sigma
    
    New_mesh.Tetra = Mesh.Tetra(find(Mesh.Sigma == unique_sigma(i)),:);
    h(i) = subplot(3,3,i)
    DisplayBoundaries(New_mesh);
end

linkprop(h,{'CameraPosition', 'CameraViewAngle', 'CameraTarget','Xlim','Ylim','Zlim'})