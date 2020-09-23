function [status,cmdout] = runmesher(inrfile,electrodesfile,parameterfile,outdir,outname,MesherRoot)
% RUNMESHER Calls Mesher from Command line from MATLAB, uses WSL if using
% Windows
%   Builds the command string and calls the Mesher. If on Windows, it will
%   call the Mesher using the default WSL distribution.
%
% The mesher is called from the command line with the following parameters
% ./bin/mesher 
%         -i input image file
%         -e electrode position file
%         -p parameter file
%         -o output mesh name (default = new_mesh)
%         -d output directory (default = output/)
%
% Inputs:
% inrfile        - path to .inr file with binary volume
% electrodesfile - path to electrodes.txt with 3D coordinates of electrode positiosn
% parameterfile  - path to param file e.g. made by writemesherparam()
%
% Optional inputs:
% outdir         - output dir, if blank or missing will use MesherRoot/output
% outname        - output mesh name, will use new_mesh if not given
% MesherRoot     - Mesher repository root. By default it assumes the mesher
%                  is built in the same root directory as this current
%                  Matlab script. The user can specify a different
%                  directory. This is useful in some cases using WSL
%
% Usage:
% from run_examples.m assuming pwd is Mesher/examples:
% runmesher('brain/input.inr','brain/Electrodes.txt','brain/input_idx.txt','brain/output/','brainexample');

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
    end
    
    %go up one level
    slashidx=strfind(meshrt,'/');
    meshrt=meshrt(1:slashidx(end-1));
    
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

% use WSL if we are on PC. This is the *default* WSL distribution, which
% can be set by wsl --set-default 
if ispc
    CMDSTR = ['wsl ' CMDSTR];
end

disp('Calling Mesher with command string:');
disp(CMDSTR);

[status,cmdout]=system(CMDSTR,'-echo');
end

