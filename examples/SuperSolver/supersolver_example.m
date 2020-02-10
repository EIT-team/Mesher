%% SuperSolver example

% Make sure you have added Mesher/MATLAB to the path, as well as the
% SuperSolver directory 

%% Run mesher
% Settings for a lower resolution (~100k elem) output mesh for demonstration

% Get the parameter structure
P=getmesherparam;

P.facet_distance_mm=2;
P.cell_coarse_size_mm=4;

P.refine.electrodes = 1;
P.electrode_radius_mm=3;
P.cell_size_electrodes_mm=2;

% Turn off all optimisations
P.opt.exude_opt=0;
P.opt.lloyd_opt=0;
P.opt.odt_opt=0;
P.opt.perturb_opt=0;

% save the output to csv to load into matlab
P.save.save_nodes_tetra=1;
% save VTK to view it paraview
P.save.save_vtk=0;

% Move the electrode positions to ensure they are placed on the surface
P.move.move_electrodes=1;
P.move.outermost_tissue=1;

% write parameter file
writemesherparam('NNSS_param.txt',P);

% Run mesher
runmesher('../neonatescalp/NNscalp.inr','../neonatescalp/NNscalp_elecINRpos.txt',...
    'NNSS_param.txt','output/','NNSS')
%% View Mesh and check electrodes

% Check electrodes are aligned correctly, and output is sensible before
% running computationally expensive output

M=loadmesh('output/NNSS');

figure
hold on;
trep = triangulation(M.Tetra, M.Nodes);
[Triangle_Boundary, Nodes_Boundary] = freeBoundary(trep);
h= trisurf(Triangle_Boundary, Nodes_Boundary(:,1), Nodes_Boundary(:,2), Nodes_Boundary(:,3));

set(h,'EdgeColor',[0.3,0.3,0.3],'FaceColor','w','FaceAlpha',1);
daspect([1,1,1]);

plot3(M.elec_pos(:,1),M.elec_pos(:,2),M.elec_pos(:,3),'.','Markersize',40);

plot3(M.gnd_pos(:,1),M.gnd_pos(:,2),M.gnd_pos(:,3),'.','Markersize',40);

hold off
view(3)
title('Neonate Mesh Low Res')
%% Setup system

%get protocol. each row is a separate measurement. with the columns CS+ CS-
%V+ V-
Prt=dlmread('NN2016_Prt_full.txt');

cond_scalp=0.2;
cond_skull=0.03;

%create conductivity vector
mat_ref=double(M.mat_ref);
mat_ref(M.mat_ref==1) = cond_scalp;
mat_ref(M.mat_ref==2) = cond_skull;

%% Run Supersolver

% get the structures with defaults
[Mesh,Fem,Fwd,Inv,Sol] = supersolver_init(M.Nodes,M.Tetra,mat_ref,M.elec_pos,M.gnd_pos,Prt);

%change the electrode diameters etc.
Fem.current                         =   240e-6; % uA
Fem.elec_diam                       =   8e-3;% electrode diameter in meters
Fem.zc                              =   100 ; % contact impedance (could be a vector at the length of the number of electrodes);

%Setup system
[Mesh,Fem,Fwd,Inv,Sol] = supersolver_setup(Mesh,Fem,Fwd,Inv,Sol);
% run fwd
[Mesh,Fem,Fwd,Inv,Sol,Data] = supersolver_runfwd(Mesh,Fem,Fwd,Inv,Sol);
% get jac
[Mesh,Fem,Fwd,Inv,Sol,J] = supersolver_solve(Mesh,Fem,Fwd,Inv,Sol);


%%
figure
plot(Data.bnd_v)
title('Simulated voltaged from SuperSolver')
xlabel('Measurement');
ylabel('Voltage (V)');
% ylim([-0.05 0.05])
saveas(gcf,'figures/SuperSolver_Volts.png')

