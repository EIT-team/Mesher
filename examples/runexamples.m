
%% Neonate brain

runmesher('neonatescalp/NNscalp.inr','neonatescalp/NNscalp_elecINRpos.txt',...
    'neonatescalp/NNscalp_param.txt','neonatescalp/output/','NNexample')

%% Unit cube

runmesher('unitcube/unit_cube.inr','unitcube/electrodes.txt',...
    'unitcube/input_idx.txt','unitcube/output/','unitcubeexample')

%% Brain

runmesher('brain/input.inr','brain/Electrodes.txt','brain/input_idx.txt','brain/output/','brainexample');