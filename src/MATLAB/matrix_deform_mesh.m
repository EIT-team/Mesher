mesh_file = 'Mesh_4p5_mln_forward.mat';
load(mesh_file);

do_transform = 1;

transform_matrix = [    1       0.1     0.1     0;
                        0.1     1       0.1     0;
                        0.1     0.1     1       0;
                        0       0       0       1;
                    ];

Mesh.Nodes(:,4) = 1;

if do_transform
    Mesh.Nodes = Mesh.Nodes*transform_matrix;
end

%Mesh.Tetra=Mesh.Tetra(find(Mesh.mat_ref == 2),:);
DisplayBoundaries(Mesh)
