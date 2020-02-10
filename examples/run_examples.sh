  echo "Running Unit Cube Example"
  mkdir unitcube/output
  ../bin/mesher -i unitcube/unit_cube.inr -e unitcube/electrodes.txt  -p unitcube/Params_0.txt -o unitcubeexample -d unitcube/output/

  echo "Running Neonate Example"
  mkdir neonatescalp/output
  ../bin/mesher -i  neonatescalp/NNscalp.inr -e neonatescalp/NNscalp_elecINRpos.txt  -p neonatescalp/NNscalp_param.txt -o NNexample -d neonatescalp/output/

  echo "Running Brain Example"
  mkdir brain/output
  ../bin/mesher -i brain/Brain.inr -e brain/Brain_elec.txt -p brain/Brain_params.txt -o brainexample -d brain/output/

  echo "Running Deformation Example"
  mkdir deformation/output
  ../bin/mesher -i unitcube/unit_cube.inr -e unitcube/electrodes.txt -p deformation/deformation_params.txt -o unitcube_deformation -d deformation/output/
  
  echo "Running refinement Examples"
  ../bin/mesher -i refinements/Input.inr -e refinements/Elecs.txt -p refinements/Params_0.txt -d refinements/output/ -o 0_Initial
  ../bin/mesher -i refinements/Input.inr -e refinements/Elecs.txt -p refinements/Params_1.txt -d refinements/output/ -o 1_Elecs
  ../bin/mesher -i refinements/Input.inr -e refinements/Elecs.txt -p refinements/Params_2.txt -d refinements/output/ -o 2_Depth
  ../bin/mesher -i refinements/Input.inr -e refinements/Elecs.txt -p refinements/Params_3.txt -d refinements/output/ -o 3_Sphere
  ../bin/mesher -i refinements/Input.inr -e refinements/Elecs.txt -p refinements/Params_4.txt -d refinements/output/ -o 4_Cuboid
  ../bin/mesher -i refinements/Input.inr -e refinements/Elecs.txt -p refinements/Params_5.txt -d refinements/output/ -o 5_Planar
