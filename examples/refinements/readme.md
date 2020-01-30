# Mesher refinement options

Here we explore the different methods of refining the mesh that are available to the user, and describe use cases for each. For simplicity we will be using a single layer mesh.

## 0. Initial Mesh - no refinement

Starting with an initial unrefined head mesh using `Params_0.txt`. All refinements are disabled by the setting the relevant flags to 0. The parameter `cell_coarse_size_mm` determines the baseline element size. The electrode file must be included even if the electrodes are not refined, as it required for the output mesh files.

```c++
facet_angle_deg = 30.000
facet_distance_mm = 2.000
cell_radius_edge_ratio = 3.000
cell_coarse_size_mm = 5.000
```

We have disabled optimisations for speed, but it is recommended to run the Mesher again with them turned on when you have found the settings you are happy with to smooth the transitions between element sizes.

```c++
electrode_refinement = 0
depth_refinement = 0
planar_refinement = 0
cuboid_refinement = 0
sphere_refinement = 0
```

Which can then be called with the following:

```bash
../../bin/mesher -i input.inr -e Elecs.txt -p Params_0.txt -o 0_Initial
```

Produces the following 25k element mesh with uniform elemtent size:
![Initial head mesh](figures/0_PV.png)

## 1. Electrode refinement

Typically a higher mesh density is required at the electrodes due to the higher field gradients and increased sensitivity. This is acheived by increasing the sizing field value in a sphere around each electrode centre. The centres are specified in the `-e Elecs.txt` file, and the following options must be set in the parameter file:

```c++
electrode_refinement = 1
electrode_radius_mm = 5.000
cell_size_electrodes_mm = 2.000
```

The mesher can then be called, and the and the denser regions are clearly visible in the output mesh.

```bash
../../bin/mesher -i input.inr -e Elecs.txt -p Params_1.txt -o 1_Elecs
```

![Mesh with electrode refinement](figures/1_Slice.png)

## 2. Depth refinement

As the sensitivity decreases with distance from the electrodes, less elements are required towards the centre of the volume. Therefore, as outlined [a methods paper](https://doi.org/10.1088/0967-3334/35/6/1095) it is possible to specify a linear decrease from the surface to the centre. This is particularly useful in applications like the brain or thorax where the elec

adasdsdasd

```c++
elements_with_fine_sizing_field_percentage = 50.000
cell_fine_size_mm = 5.000
cell_coarse_size_mm = 5.000
```

The mesher can then be called:

```bash
../../bin/mesher -i input.inr -e Elecs.txt -p Params_2.txt -o 2_Depth
```

The gradient 

![Mesh with depth refinement](figures/2_Slice.png)

## 3. Sphere refinement

A separate sphere anywhere in the mesh, separate from the electrodes, can be defined through the following parameters:

```c++
sphere_refinement = 1
sphere_radius = 20.000
sphere_centre_x = 60.00
sphere_centre_y = 50.00
sphere_centre_z = 60.00
sphere_cell_size = 2.000
```

All inputs are specified in mm, and are in the coordinates of the final mesh *i.e.* not scaled by vx,vy,vz. Defining the element size within a volume like this is useful either to 1) *increase* the mesh density around internal electrodes like [MAYO PAPER](MAYOS), or 2) to explicitly decrease mesh density in the centre similar to depth refinement.

```bash
../../bin/mesher -i input.inr -e Elecs.txt -p Params_3.txt -o 3_Sphere
```

![Mesh with sphere refined region](figures/3_Slice.png)

## 4. Cuboid refinement

A cuboid of constant element size can also be definied in the same manner as the sphere. All inputs are in mm with respect to the final mesh (not scaled by vx,vy,vz). "Extent" is the distance along one axis from the centre to the edge, so the cube in this case has WxLxH of 60x20x20.

```c++
cuboid_refinement = 1
cuboid_x_extent = 30.000
cuboid_y_extent = 10.000
cuboid_z_extent = 10.000
cuboid_centre_x = 60.00
cuboid_centre_y = 50.200
cuboid_centre_z = 60.900
cuboid_cell_size = 2
```

```bash
../../bin/mesher -i input.inr -e Elecs.txt -p Params_4.txt -o 4_Cuboid
```

![Mesh with cuboid refined region](figures/4_Slice.png)

## 5. Planar refinement


Note, it is not recommended to use planar and depth refinement at the same time.

```bash
../../bin/mesher -i input.inr -e Elecs.txt -p Params_5.txt -o 5_Planar
```
