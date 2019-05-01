# Calling Mesher from MATLAB


```matlab
P = getmesherparam(); % get mesher parameters structure
P.electrode_radius_mm=3; % change the settings as you need
writemesherparam('exampleparameters.txt',P) % write parameter file
runmesher('Example.inr','ExampleElecPos.txt','exampleparameters.txt');%call mesher

```
This will write the following files:
```
new_mesh.dgf
new_mesh.electrodes
new_mesh.parameters
new_mesh.vtu (optional)
new_mesh_tetra.csv (optional)
new_mesh_verticies.csv (optional)
```


# Reading output into MATLAB

```matlab
[Mesh] = loadmesh('new_mesh'); %use the "base" name without any extension (i.e. what is specified using the -o parameter)
```

# Converting STL to INR#
Until the Mesher can handle surfaces, the STL file mush be converted to INR

```matlab
[binmask,elec_pos]=stl2inr('Example.stl',4,ExampleElecPos);
```