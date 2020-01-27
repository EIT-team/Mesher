%% Unit Cube example MATLAB scripts

% Call the EIT-MESHER with two different parameter scripts

% unoptimised initial mesh
runmesher('unit_cube.inr','electrodes.txt', 'Params_0.txt','output/','0_Cube');
%%
% Optimised mesh, also saving to .csv to load into MATLAB
runmesher('unit_cube.inr','electrodes.txt', 'Params_1.txt','output/','1_CubeOpt');

%% Plot Output Mesh

Mesh=loadmesh('output/1_CubeOpt'); %requires tetra to be saved

%find the surface triangles and create triangulation
trep = triangulation(Mesh.Tetra, Mesh.Nodes);
[Triangle_Boundary, Nodes_Boundary] = freeBoundary(trep);

figure
hold on;
%plot surface mesh
h= trisurf(Triangle_Boundary, Nodes_Boundary(:,1), Nodes_Boundary(:,2), Nodes_Boundary(:,3));
set(h,'EdgeColor',[0.3,0.3,0.3],'FaceColor','w','FaceAlpha',0);
daspect([1,1,1]);
%add electrodes
plot3(Mesh.elec_pos(:,1),Mesh.elec_pos(:,2),Mesh.elec_pos(:,3),'.','Markersize',40);

hold off
view(3)
title('Unit Cube Mesh')

saveas(gcf,'figures/CubeMatlab.png')
