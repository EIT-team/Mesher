function [ full_mask,elec_pos_new_sc,transform ] = stl2inr( stlfile,pixel_scale,elec_pos )
%STL2INR Convert stl file to inr format for meshing
%   STL files output from solidworks, are converted into volumetric mask.
%   Electrode positions in original STL coordinates are converted to the
%   inr coordinates. User specifies the pixel_scale, higher creates a
%   denser inr mask


% Dependencies:
% iso2mesh http://iso2mesh.sourceforge.net/cgi-bin/index.cgi
% stlTools https://uk.mathworks.com/matlabcentral/fileexchange/51200-stltools or
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
vol_res=1/pixel_scale;

% Check if we want to plot mesh afterwards
if exist('plotflag','var') == 0 || isempty(plotflag)
    plotflag =1;
end

% get savepath
[savepath,savename]=fileparts(stlfile);

if isempty(savepath)
    savefullpath=[savename];
else
    savefullpath=[savepath filesep savename];
end

%% Loading stls

if isstruct(stlfile)
    stlsurf.vertices = stlfile.vtx;
    stlsurf.faces = stlfile.tri;
    stlfile = stlfile.name;
else
    disp('Loading Stl meshes');
    [stlsurf.vertices,stlsurf.faces,stlsurf.normals,stlsurf.name] = stlRead(stlfile);
end

if plotflag
    figure;
    hold on
    h=plotmesh(stlsurf.vertices,stlsurf.faces,'facealpha',0);
    plot3(elec_pos(:,1),elec_pos(:,2),elec_pos(:,3),'.','MarkerSize',30);
    for iElec=1:size(elec_pos,1)
        text(elec_pos(iElec,1),elec_pos(iElec,2),elec_pos(iElec,3),num2str(iElec),'FontWeight','Bold');
    end
    hold off
    set(h,'EdgeColor',[0.8,0.8,0.8],'FaceColor',[1,1,1],'FaceAlpha',0.5,'EdgeAlpha',1)
    title(sprintf('Stl file %s, elecs in original positions',stlfile),'Interpreter','none');
    drawnow
end
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

if plotflag
    figure
    imagesc(full_mask(:,:,mask_mid_idx(3)));daspect([1,1,1])
    title('Z slice through centre of binary mask');
    drawnow
end
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
if plotflag
    figure
    title('Elecs on new isosurface - check alignment here!');
    hold on
    patch('Vertices',node,'faces',elem,'FaceColor','none','EdgeAlpha',0.2,'EdgeColor',[0.8,0.8,0.8]);
    
    plot3(elec_pos_new_sc(:,1),elec_pos_new_sc(:,2),elec_pos_new_sc(:,3),'.','MarkerSize',20);
    
    for iElec=1:size(elec_pos,1)
        text(elec_pos_new_sc(iElec,1),elec_pos_new_sc(iElec,2),elec_pos_new_sc(iElec,3),num2str(iElec),'FontWeight','Bold');
    end
    
    daspect([1,1,1])
    
    hold off
end
%% save stuff for mesher

full_mask=uint8(full_mask); % inr files need uint8

%save the volumetric data for both skull and no skull cases
saveinr_EIT(uint8(full_mask),[savefullpath '.inr'],vol_res*[1 1 1]);
% save the electrode locations in the coordinates of the inr
dlmwrite([savefullpath '_elecINRpos.txt'],elec_pos_new_sc);

transform=stlsurf.transform;

end

