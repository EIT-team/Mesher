# Example meshes

The given examples have been tested on Ubuntu, either natively or running through Windows Subsystem for Linux. The parameters used for each example are set to run quickly/produce small meshes. More realistic parameters, which will generate higher resolution meshes are also provided (e.g. `Brain_params_HR.txt`)

See `runexamples.m` for usage of the MATLAB wrappers.

## [Unit Cube](unitcube/)

The simplest "Hello world" example.

 ```bash
 ../bin/mesher -i unitcube/unit_cube.inr -e unitcube/electrodes.txt  -p unitcube/input_idx.txt -o unitcubeexample -d unitcube/output/
 ```

![Unit Cube](unitcube/figures/CubeOpt.png)

## [Neonate scalp](neonatescalp/)

Two layer mesh from STL.

```bash
../bin/mesher -i  neonatescalp/NNscalp.inr -e neonatescalp/NNscalp_elecINRpos.txt  -p neonatescalp/NNscalp_param.txt -o NNexample -d neonatescalp/output/
```

![Neonate Scalp](neonatescalp/figures/NN_HR_PV.png)

## [Brain](brain/)

Mesh from MRI segmentation

```bash
../bin/mesher -i brain/input.inr -e brain/Electrodes.txt -p brain/input_idx.txt -o brainexample -d brain/output/
```

![Brain](brain/figures/brain_PV_HR.png)

## [Refinements](refinements/)

Overview of the different refinement options

![Refinements](refinements/figures/3_Slice.png)

## Deformation

## Using the Mesher output

The folder [solvers](./solvers) contains of examples of how to use the MESHER output with EIT and DOT software

### EIT - PEITS, EIDORS, SUPERSOLVER

![PEITS_Volts](solvers/PEITS/figures/PEITS_Volts.png)

### DOT - Toast++

![PEITS_Volts](solvers/toast/figures/Toast_Phi.png)