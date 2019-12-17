function [ full_mask,elec_pos_new_sc ] = stl2inr( stlfile,pixel_scale,elec_pos )
%STL2INR Convert stl file to inr for meshing
%   Detailed explanation goes here
% requires iso2mesh and stlTools
% https://uk.mathworks.com/matlabcentral/fileexchange/51200-stltools or
% inside this folder

%% check dependencies

%stl reader
if isempty(which('stlRead'))
    p=mfilename('fullpath');
    p=fileparts(p);
    addpath([p filesep 'stlTools']);
end

% iso2mesh is needed 
if (isempty(which('plotmesh')) || isempty(which('surf2vol')) || isempty(which('binsurface'))) 
    error('stl2inr needs iso2mesh');
    
end

%% check inputs
% resolution of output volumetric image
% pixel_scale = 1/vol_res; % THIS IS WHAT MUST MATCH IN THE MESHER SETTINGS
vol_res=1/pixel_scale;


%% Loading stls
disp('Loading Stl meshes');
[stlsurf.vertices,stlsurf.faces,stlsurf.normals,stlsurf.name] = stlRead(stlfile);

figure;
hold on
plotmesh(stlsurf.vertices,stlsurf.faces,'facealpha',0)
plot3(elec_pos(:,1),elec_pos(:,2),elec_pos(:,3),'.','MarkerSize',30);
hold off
title(sprintf('Stl file %s, elecs in original positions',stlfile));
drawnow
%% convert to binary mask

disp('Making binary masks')

%define the mask range - this forces all the masks to be the same size
mask_range= [floor(min([ stlsurf.vertices]))-vol_res; ceil(max([stlsurf.vertices]))+vol_res];

%find the middle for plotting the slices
mask_mid_idx = round(((mask_range(2,:) - mask_range(1,:))/2)/vol_res) ;

%convert to binary mask mask
[stlsurf.mask, stlsurf.transform] =surf2vol(stlsurf.vertices,stlsurf.faces,mask_range(1,1):vol_res:mask_range(2,1),mask_range(1,2):vol_res:mask_range(2,2),mask_range(1,3):vol_res:mask_range(2,3),'fill',1);
stlsurf.mask_sc=stlsurf.mask;

% combine masks
full_mask = stlsurf.mask_sc;

figure
imagesc(full_mask(:,:,mask_mid_idx(3)));daspect([1,1,1])
title('Z slice through centre of binary mask');
drawnow

%% convert electode positions
%adjust positons based on the affine transform output from surf2vol
newx=elec_pos(:,1) -stlsurf.transform(1,4);
newy=elec_pos(:,2) -stlsurf.transform(2,4);
newz=elec_pos(:,3) -stlsurf.transform(3,4);

elec_pos_new=[newx newy newz];
elec_pos_new_sc=elec_pos_new*pixel_scale; % scale the electrode positions

%% Check alignment of mask and electrodes

%compare electrode positions

[node,elem]=binsurface(full_mask,4); % get just the surface of the binary mask

figure
title('Elecs on new isosurface - check alignment here!');
hold on
patch('Vertices',node,'faces',elem,'FaceColor','none','EdgeAlpha',0.2);

plot3(elec_pos_new_sc(:,1),elec_pos_new_sc(:,2),elec_pos_new_sc(:,3),'.','MarkerSize',30);

daspect([1,1,1])

hold off
%% save stuff for mesher

full_mask=uint8(full_mask); % inr files need uint8

[savepath,savename]=fileparts(stlfile);

%save the volumetric data for both skull and no skull cases
saveinr_EIT(uint8(full_mask),[savepath filesep savename '.inr'],vol_res*[1 1 1]);
% save the electrode locations in the coordinates of the inr
dlmwrite([savepath filesep savename '_elecINRpos.txt'],elec_pos_new_sc);


end

