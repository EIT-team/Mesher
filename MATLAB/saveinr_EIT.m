function saveinr_EIT(vol,fname, vxyz)
%
% saveinr_EIT(vol,fname, vxyz)
%
% save a surface mesh to INR Format
%
% author: Qianqian Fang, <q.fang at neu.edu>
% date: 2009/01/04
%
% input:
%      vol: input, a binary volume
%      fname: output file name
%      vxyz: 3 element array with  mesh spacing in x, y and z dimensions.
%      Values default to 1 if not supplied.
% -- this function is part of iso2mesh toolbox (http://iso2mesh.sf.neta)
%
% Modified by T Dowrick 23.5.2017 to allow specifying the values of vx, vy, vz

%disp(["This is a modified version of saveinr which allows specificying non uniform vx, vy, vz"])

if (nargin < 3)
    vx = 1;
    vy = 1;
    vz = 1;
    
else
    vx = vxyz(1);
    vy = vxyz(2);
    vz = vxyz(3);
    
end

fid=fopen(fname,'wb');
if(fid==-1)
    error('You do not have permission to save mesh files.');
end
dtype=class(vol);
if(islogical(vol) | strcmp(dtype,'uint8'))
    btype='unsigned fixed';
    dtype='uint8';
    bitlen=8;
elseif(strcmp(dtype,'uint16'))
    btype='unsigned fixed';
    dtype='uint16';
    bitlen=16;
elseif(strcmp(dtype,'float'))
    btype='float';
    dtype='float';
    bitlen=32;
elseif(strcmp(dtype,'double'))
    btype='float';
    dtype='double';
    bitlen=64;
else
    error('volume format not supported');
end
header=sprintf(['#INRIMAGE-4#{\nXDIM=%d\nYDIM=%d\nZDIM=%d\nVDIM=1\nTYPE=%s\n' ...
    'PIXSIZE=%d bits\nCPU=decm\nVX=%d\nVY=%d\nVZ=%d\n'],size(vol),btype,bitlen, vx, vy ,vz);
header=[header char(10*ones(1,256-4-length(header))) '##}' char(10)];
fwrite(fid,header,'char');
fwrite(fid,vol,dtype);
fclose(fid);
