# Example meshes

See `runexamples.m` for usage of the MATLAB wrappers.

Add `wsl` before each of these commands to call it from windows subsystem for linux

The parameters have been changed to produce small meshes, but realistic meshing parameters are also given.

## Unit Cube

 ```bash
 ../bin/mesher -i unitcube/unit_cube.inr -e unitcube/electrodes.txt  -p unitcube/input_idx.txt -o unitcubeexample -d unitcube/output/
 ```

![Unit Cube](unitcube/figures/CubeOpt.png)

## Neonate scalp

```bash
../bin/mesher -i  neonatescalp/NNscalp.inr -e neonatescalp/NNscalp_elecINRpos.txt  -p neonatescalp/NNscalp_param.txt -o NNexample -d neonatescalp/output/
```

![Neonate Scalp](neonatescalp/figures/NN_HR_PV.png)

## Brain

```bash
../bin/mesher -i brain/input.inr -e brain/Electrodes.txt -p brain/input_idx.txt -o brainexample -d brain/output/
```

![Brain](brain/figures/brain_PV_HR.png)

## Refinements

![Refinements](refinements/figures/3_Slice.png)

## Deformation
