function [status,cmdout] = runmesher(inrfile,electrodesfile,parameterfile,outdir,outname,MesherRoot)
%RUNMESHER Calls Mesher from Command line from MATLAB, uses WSL if using
%Windows
%   Detailed explanation goes here

%% Mesher location

% use the default MesherRoot if one is not specified - the root of the
% current function.
if exist('MesherRoot','var') == 0 || isempty(MesherRoot)
    
    meshrt=mfilename('fullpath');
    % get the current mesher root
    
    %If we are using WSL then we want to change to linux path,
    % e.g. 'C:\Users\James\Mesher\MATLAB' to '/mnt/c/Users/James/Mesher/'
    
    if ispc
        meshrt=strrep(meshrt,'\','/');
        meshrt=strrep(meshrt,':','');
        meshrt(1)=lower(meshrt(1));
        meshrt=['/mnt/' meshrt];
        
        slashidx=strfind(meshrt,'/');
        meshrt=meshrt(1:slashidx(end-1));
    end
    
    % e.g. %'/mnt/c/Users/djave/Mesher/' if you are using WSL;
    MesherRoot = meshrt;
end

MesherDir = 'bin/mesher';

%% Inputs

INRSTR = inrfile;
% [MesherRoot 'inputs/input.inr'];

ELECSTR = electrodesfile;
% [MesherRoot 'inputs/Electrodes.txt'];

PARAMSTR = parameterfile;
% [MesherRoot 'inputs/input_idx.txt'];


CMDSTR = [MesherRoot MesherDir ' -i ' INRSTR ' -e ' ELECSTR ' -p ', PARAMSTR];

%% Optional inputs

% output directory

if exist('outdir','var') == 1 && ~isempty(outdir)
    DIRSTR=outdir; 
    CMDSTR=[CMDSTR ' -d ' DIRSTR];
else
    outdir='./output/';
end

% Check out directory exists
if ~isfolder(outdir)
    mkdir(outdir);
end

%Mesh name
if exist('outname','var') == 1 && ~isempty(outname)
    NAMESTR=outname;
    CMDSTR=[CMDSTR ' -o ' NAMESTR];
end

%% Finally call mesher

% use WSL if we are on PC
if ispc
    CMDSTR = ['wsl ' CMDSTR];
end

disp('Calling Mesher with command string:');
disp(CMDSTR);

[status,cmdout]=system(CMDSTR,'-echo');
end

