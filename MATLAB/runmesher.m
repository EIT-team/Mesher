function [status,cmdout] = runmesher(inrfile,electrodesfile,parameterfile,outdir,outname,MesherRoot)
%RUNMESHER Calls Mesher from command line from windows, having installed it
%on WSL
%   Detailed explanation goes here

%% Mesher location

% ./bin/mesher -i inputs/input.inr -e inputs/Electrodes.txt -p inputs/input_idx.txt

[status,cmdout]=system('wsl pwd','-echo');


% use the default MesherRoot if one is not specified - the root of the
% current function.
if exist('MesherRoot','var') == 0 || isempty(MesherRoot)
    
    % get the current mesher root and change to linux path
    meshrt=mfilename('fullpath');
    meshrt=strrep(meshrt,'\','/');
    meshrt=strrep(meshrt,':','');
    meshrt(1)=lower(meshrt(1));
    meshrt=['/mnt/' meshrt];
    
    slashidx=strfind(meshrt,'/');
    meshrt=meshrt(1:slashidx(end-1));
    
    % e.g. %'/mnt/c/Users/djave/Mesher/';
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


CMDSTR = [];

CMDSTR = [MesherRoot MesherDir ' -i ' INRSTR ' -e ' ELECSTR ' -p ', PARAMSTR];

%% Optional inputs


% output directory

% Check out directory exists
if exist('outdir','var') == 1 && ~isempty(outdir)
    if ~isfolder(outdir)
        mkdir(outdir);
    end
    
    DIRSTR=outdir;
    
    CMDSTR=[CMDSTR ' -d ' DIRSTR];
    
end

%Mesh name
if exist('outname','var') == 1 && ~isempty(outname)
    
    NAMESTR=outname;
    CMDSTR=[CMDSTR ' -o ' NAMESTR];
    
end

%% Finally call mesher

CMDSTR = ['wsl ' CMDSTR];


disp('Calling Mesher with command string:');
disp(CMDSTR);

[status,cmdout]=system(CMDSTR,'-echo');



end

