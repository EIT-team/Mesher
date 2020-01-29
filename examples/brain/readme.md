# Rat Brain Mesh

A typical use case is to create a mesh from an MRI/CT segmentation. Here we have a rat brain with the white and grey matter segmented as separate layers using [Seg3D](http://seg3d.org). In this example is important to **resample** the binary mask to have **uniform spacing** in the x y z directions. This makes defining electrode and refinement locations simpler.

![Rat Brain Mesh Paraview](figures/Seg3D.png)

Cortical electrode locations were defined within the INR coordinate space, as shown in `example_brain.m`.

![Rat Brain Mesh Paraview](figures/brain_seg.png) ![Rat Brain Mesh Paraview](figures/brain_seg_slice.png)

## Run the mesher
The parameters defined in `Brain_params.txt` provide an appoixmately uniform sizing field throughout the brain, with refinement around the electrodes. As the electrode positions were defined in the INR space, the final electrode positions are shifted to the outer surface of the mesh by setting the parameters `move_electrodes = 1` and `outermost_tissue = 2`.

```c++
#Standard refinement parameters
refine_electrodes = 1 // turn on electrode refinement
pixel_scale_mm = 8.000 // resolution of inr file, 8 pixels per mm
facet_angle_deg = 30.000 // limit angle and size
facet_distance_mm = 0.400 
cell_radius_edge_ratio = 3.000
cell_fine_size_mm = 0.500 
cell_coarse_size_mm = 0.600
electrode_radius_mm = 0.300
cell_size_electrodes_mm = 0.100 // smaller elements around elecs
```

The mesher can then be called directly using the following:

```bash
../../bin/mesher -i Brain.inr -e Brain_elec.txt -p Brain_params.txt -d output/ -o BrainMesh
```

![Rat Brain Mesh Paraview](figures/brain_PV.png)

The electrode alignment and refinement regions are also clearly visible in MATLAB.

```matlab
Mesh=loadmesh('output/BrainMesh');
figure
hold on;
trep = triangulation(Mesh.Tetra, Mesh.Nodes);
[Triangle_Boundary, Nodes_Boundary] = freeBoundary(trep);
h= trisurf(Triangle_Boundary, Nodes_Boundary(:,1), Nodes_Boundary(:,2), Nodes_Boundary(:,3));
set(h,'EdgeColor',[0.3,0.3,0.3],'FaceColor','w','FaceAlpha',1);
daspect([1,1,1]);
plot3(Mesh.elec_pos(:,1),Mesh.elec_pos(:,2),Mesh.elec_pos(:,3),'.','Markersize',30);
hold off
view(3)
title('Rat Brain Mesh Low Res')
```

![Rat Brain Mesh MATLAB with elecs](figures/BrainMesh_LowRes.png)


## High Resolution Mesh

Once the aligment and extend of the refinements have been established, a higher resolution mesh with post processing optimisations can be created using the `Brain_params_HR.txt` parameter file. 

```
refine_electrodes = 1
pixel_scale_mm = 8.000
facet_angle_deg = 30.000
facet_distance_mm = 0.400
cell_radius_edge_ratio = 3.000
cell_fine_size_mm = 0.150
cell_coarse_size_mm = 0.200
electrode_radius_mm = 0.300
cell_size_electrodes_mm = 0.050
```
This creates an ~2.5mln element mesh, with highly refined electrodes.

![Rat Brain Mesh Paraview](figures/brain_PV_HR.png)