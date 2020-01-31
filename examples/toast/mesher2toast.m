%% MESHER 2 TOAST++
% Simple example of how to convert output from Mesher into Toast++ format
% Requires Toast to be installed

P=getmesherparam;

P.refine.electrodes=1;
P.refine.depth=1;

P.cell_coarse_size_mm=8;
P.cell_fine_size_mm=2;
P.elements_with_fine_sizing_field_percentage=80;
P.electrode_radius_mm=5;
P.cell_size_electrodes_mm=1;

writemesherparam('Params_Toast',P);

runmesher('../refinements/input.inr','../refinements/Elecs.txt','Params_Toast.txt','output','ToastEx');