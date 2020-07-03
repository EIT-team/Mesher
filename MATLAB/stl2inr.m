function [ varargout ] = stl2inr( STLin,pixel_scale,elec_pos,plotflag )
%STL2INR Convert stl file to inr format for meshing
%   STL files output from solidworks, are converted into volumetric mask.
%   Electrode positions in original STL coordinates are converted to the
%   inr coordinates. User specifies the pixel_scale, higher creates a
%   denser inr mask.
%
%   Inputs:
%   STLin - STL filename(s). String input for single file, or cell array
%   for multiple. Additionally a single struct can be passed instead, to
%   avoid having to open the file repeatedly
%
%   pixel_scale - resolution of the resultant inr in the units of the input
%   stl (usually mm). Pixel_scale of 5 will create 5 voxels for every mm.
%
%   elec_pos - electrode positions in original STL coordinates if missing
%   no scaling happens


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

% Check if electrodes are specified
if exist('elec_pos','var') == 0 || isempty(elec_pos)
    elecflag =0;
else
    elecflag=1;
end

% Check if we want to plot mesh afterwards
if exist('plotflag','var') == 0 || isempty(plotflag)
    plotflag =1;
end
%% Loading stls

if iscell(STLin)
    stlnum=size(STLin,2);
    % load each of the files into a struct
    for iFile=1:stlnum
        [stlsurf(iFile).vertices,stlsurf(iFile).faces,stlsurf(iFile).normals,stlsurf(iFile).name] = stlRead(STLin{iFile});
    end
    % get savepath
    [savepath,savename]=fileparts(STLin{1});
else % if only one input specified
    stlnum=1;
    if isstruct(STLin)
        stlsurf.vertices = STLin.vtx;
        stlsurf.faces = STLin.tri;
        savepath = STLin.path;
        savename = STLin.name;
    else
        disp('Loading Stl meshes');
        [stlsurf.vertices,stlsurf.faces,stlsurf.normals,stlsurf.name] = stlRead(STLin);
        % get savepath
        [savepath,savename]=fileparts(STLin);
    end
end

if plotflag
    figure;
    hold all
    for iMesh =1:stlnum
        h=plotmesh(stlsurf(iMesh).vertices,stlsurf(iMesh).faces,'facealpha',0);
        set(h,'EdgeColor',[0.8,0.8,0.8]-0.1*(iMesh-1),'FaceColor',[1,1,1],'FaceAlpha',0.5,'EdgeAlpha',1)
    end
    if elecflag
        plot3(elec_pos(:,1),elec_pos(:,2),elec_pos(:,3),'.','MarkerSize',30);
        for iElec=1:size(elec_pos,1)
            text(elec_pos(iElec,1),elec_pos(iElec,2),elec_pos(iElec,3),num2str(iElec),'FontWeight','Bold');
        end
    end
    hold off
    title(sprintf('Stl file %s, elecs in original positions',savename),'Interpreter','none');
    drawnow
end

if isempty(savepath)
    savefullpath=[savename];
else
    savefullpath=[savepath filesep savename];
end

%% Find the mask range

% disp('Making binary masks')

for iMesh=1:stlnum
    minvert(iMesh,:)=min(stlsurf(iMesh).vertices);
    maxvert(iMesh,:)=max(stlsurf(iMesh).vertices);
end
%hack for correct behaviour with 1 stl
minvert=[minvert; minvert];
maxvert=[maxvert; maxvert];

%define the mask range - this forces all the masks to be the same size
mask_range= [floor(min(minvert))-vol_res; ceil(max(maxvert))+vol_res];

%find the middle for plotting the slices
mask_mid_idx = round(((mask_range(2,:) - mask_range(1,:))/2)/vol_res) ;

%% Make binary masks
%convert all stls to binary masks
for iMesh=1:stlnum
    % range is defined above
    [stlsurf(iMesh).mask, stlsurf(iMesh).transform] =surf2vol(stlsurf(iMesh).vertices,stlsurf(iMesh).faces,mask_range(1,1):vol_res:mask_range(2,1),mask_range(1,2):vol_res:mask_range(2,2),mask_range(1,3):vol_res:mask_range(2,3),'fill',1);
end

% combine masks overwriting values
full_mask=zeros(size(stlsurf(1).mask));
for iMesh =1:stlnum
    full_mask(logical(stlsurf(iMesh).mask))=iMesh;
end

if plotflag
    figure
    imagesc(full_mask(:,:,mask_mid_idx(3)));daspect([1,1,1])
    title('Z slice through centre of binary mask');
    drawnow
end
%% convert electode positions
if elecflag
    %adjust positons based on the affine transform output from surf2vol,
    %using the first layer only (assuming this is outer or "main" layer
    newx=elec_pos(:,1) -stlsurf(1).transform(1,4);
    newy=elec_pos(:,2) -stlsurf(1).transform(2,4);
    newz=elec_pos(:,3) -stlsurf(1).transform(3,4);
    
    elec_pos_new=[newx newy newz];
    elec_pos_new_sc=elec_pos_new*pixel_scale; % scale the electrode positions
end

%% Check alignment of mask and electrodes

%compare electrode positions

[node,elem]=binsurface(full_mask,4); % get just the surface of the binary mask
if plotflag
    figure
    title('Elecs on new surface - ensure aligment before meshing');
    hold on
    patch('Vertices',node,'faces',elem,'FaceColor','none','EdgeAlpha',0.2,'EdgeColor',[0.8,0.8,0.8]);
    if elecflag
        plot3(elec_pos_new_sc(:,1),elec_pos_new_sc(:,2),elec_pos_new_sc(:,3),'.','MarkerSize',20);
        
        for iElec=1:size(elec_pos,1)
            text(elec_pos_new_sc(iElec,1),elec_pos_new_sc(iElec,2),elec_pos_new_sc(iElec,3),num2str(iElec),'FontWeight','Bold');
        end
    end
    daspect([1,1,1])
    view(3);
    hold off
    drawnow
end
%% save stuff for mesher

full_mask=uint8(full_mask); % inr files need uint8

%save the volumetric data for both skull and no skull cases
saveinr_EIT(uint8(full_mask),[savefullpath '.inr'],vol_res*[1 1 1]);

varargout{1}=full_mask;

if elecflag
    % save the electrode locations in the coordinates of the inr
    dlmwrite([savefullpath '_elecINRpos.txt'],elec_pos_new_sc);
    
    transform=stlsurf(1).transform;
    varargout{2}=elec_pos_new_sc;
    varargout{3}=transform;
    
end


disp('stl2inr Done');

end

