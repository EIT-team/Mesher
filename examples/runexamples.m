
%% Neonate brain

pixel_scale=1;

% create INR from STL
elec_pos=dlmread('neonatescalp/NNelecposorig.txt');

[ full_mask,elec_pos_new_sc ] = stl2inr('neonatescalp/NNscalp.stl',pixel_scale,elec_pos );

%set parameters
P=getmesherparam;
P.pixel_scale_mm=pixel_scale;
P.facet_distance_mm=2;
P.cell_fine_size_mm=3;
P.cell_coarse_size_mm=8;
P.electrode_radius_mm=3;
P.cell_size_electrodes_mm=2;

P.opt.exude_opt=1;
P.opt.lloyd_opt=1;
P.opt.odt_opt=1;
P.opt.perturb_opt=1;

P.save.save_nodes_tetra=0;
writemesherparam('neonatescalp/NNscalp_param.txt',P);

% Run mesher
runmesher('neonatescalp/NNscalp.inr','neonatescalp/NNscalp_elecINRpos.txt',...
    'neonatescalp/NNscalp_param.txt','neonatescalp/output/','NNexample')


%% Unit cube

runmesher('unitcube/unit_cube.inr','unitcube/electrodes.txt',...
    'unitcube/input_idx.txt','unitcube/output/','unitcubeexample')

%% Brain

runmesher('brain/input.inr','brain/Electrodes.txt','brain/input_idx.txt','brain/output/','brainexample');