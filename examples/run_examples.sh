  echo "Running Unit Cube Example"
  mkdir unitcube/output
  ../bin/mesher -i unitcube/unit_cube.inr -e unitcube/electrodes.txt  -p unitcube/Params_0.txt -o unitcubeexample -d unitcube/output/

  echo "Running Neonate Example"
  mkdir neonatescalp/output
  ../bin/mesher -i  neonatescalp/NNscalp.inr -e neonatescalp/NNscalp_elecINRpos.txt  -p neonatescalp/NNscalp_param.txt -o NNexample -d neonatescalp/output/

  echo "Running Brain Example"
  mkdir brain/output
  ../bin/mesher -i brain/Brain.inr -e brain/Brain_elec.txt -p brain/Brain_params.txt -o brainexample -d brain/output/
