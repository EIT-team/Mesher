# Example meshes

See `runexamples.m` for usage of the MATLAB wrappers. 

Add `wsl` before each of these commands to call it from windows subsystem for linux

The parameters have been changed to produce small meshes, in reality the elements should be finer

### Unit Cube
 ```
 ../bin/mesher -i unitcube/unit_cube.inr -e unitcube/electrodes.txt  -p unitcube/input_idx.txt -o unitcubeexample -d unitcube/output/
 ```

### Neonate scalp
```
../bin/mesher -i  neonatescalp/NNscalp.inr -e neonatescalp/NNscalp_elecINRpos.txt  -p neonatescalp/NNscalp_param.txt -o NNexample -d neonatescalp/output/
```
### Brain

```
../bin/mesher -i brain/input.inr -e brain/Electrodes.txt -p brain/input_idx.txt -o brainexample -d brain/output/
```


