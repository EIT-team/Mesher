%% MESHER 2 TOAST++
% Simple example of how to convert output from Mesher into Toast++ format
% Requires Toast to be installed. Currently only a homogenous nodal basis
% is used for simplicity

%% Create mesh
P=getmesherparam;

P.refine.electrodes=1;
P.refine.depth=1;

P.cell_coarse_size_mm=20;
P.cell_fine_size_mm=8;
P.elements_with_fine_sizing_field_percentage=80;
P.electrode_radius_mm=3;
P.cell_size_electrodes_mm=3;

P.opt.exude_opt=1;
P.opt.lloyd_opt=1;
P.opt.odt_opt=1;
P.opt.perturb_opt=1;

writemesherparam('Params_Toast.txt',P);

%reduce the number of electrodes as we are using single source/detector
%pair in example

elecpos=dlmread('../refinements/Elecs.txt');

dlmwrite('ToastElecs.txt',elecpos([18,19],:));


runmesher('../refinements/input.inr','ToastElecs.txt','Params_Toast.txt','output/','ToastEx');

%% load into toast

%Mesher Structure
MM=loadmesh('output/ToastEx');

%Toast Structure
Tmesh = toastMesh (MM.Nodes,MM.Tetra(:,1:4),3*ones(size(MM.Tetra,1),1)); % 3 for 4 sided tetra
% create the mesh object
ne = Tmesh.ElementCount; % get number of nodes
nn = Tmesh.NodeCount;

%% Place Sources and detectors

%33 electrode positions were previously defined, so use those but split
%them into 

Q=MM.elec_pos(1:2:end,:);
M=MM.elec_pos(2:2:end,:);

% use only a subset if we want 

q_idx=[1];
m_idx=[1];
Q=Q(q_idx,:);
M=M(m_idx,:);

%number of sources and detectors
nq=size(Q,1);
nm=size(M,1);

%put these on the mesh
Tmesh.SetQM(Q,M);

figure
hold on
h=Tmesh.Display;
h.FaceAlpha=0.8;
h1=plot3(Q(:,1),Q(:,2),Q(:,3),'r.','MarkerFaceColor','r','MarkerSize',30);
h2=plot3(M(:,1),M(:,2),M(:,3),'b.','MarkerFaceColor','b','MarkerSize',30);
hold off
legend([h1 h2],'Sources','Detectors')
%% set coeffs

%for simplicity we are using a nodal basis, wheras in reality we should use
%a grid basis made by using toastBasis

mua_bkg = 0.01; %absorption
mus_bkg = 1.0; % scattering
ref_bkg = 1.4; % refractive index
mua = ones(nn,1) * mua_bkg; % make all homogeneous
mus = ones(nn,1) * mus_bkg;
ref = ones(nn,1) * ref_bkg;

%% Solve Fwd
qvec = Tmesh.Qvec('Neumann','Gaussian',0.05); % define sources
mvec = Tmesh.Mvec('Gaussian',0.05,ref_bkg); % define detectors
Phi1 = toastFields(Tmesh,0,qvec,mua,mus,ref,0); % photon density
J = toastJacobian (Tmesh, 0, qvec, mvec, mua, mus, ref,0); % PDMF
%% Plot 
cur_source=1;

figure; 
hold on
Tmesh.Display((Phi1(:,cur_source)),'lighting',0,'showgrid',1); title('Photon Density')
h1=plot3(Q(:,1),Q(:,2),Q(:,3),'r.','MarkerFaceColor','r','MarkerSize',30);
h2=plot3(M(:,1),M(:,2),M(:,3),'b.','MarkerFaceColor','b','MarkerSize',30);
hold off
legend([h1 h2],'Sources','Detectors')
saveas(gcf,'figures/Toast_Phi.png');


figure;
hold on
Tmesh.Display((J(cur_source,1:nn)),'lighting',0,'showgrid',1); title('PMDF - J(a)');
h1=plot3(Q(:,1),Q(:,2),Q(:,3),'r.','MarkerFaceColor','r','MarkerSize',30);
h2=plot3(M(:,1),M(:,2),M(:,3),'b.','MarkerFaceColor','b','MarkerSize',30);
hold off
legend([h1 h2],'Sources','Detectors')
saveas(gcf,'figures/Toast_PDMF.png');

