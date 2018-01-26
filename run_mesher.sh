set -e # Terminate script on any error
for mesh_name in "01000_padded" "01001_padded" "01003_padded"
do
	input="./inputs/"$mesh_name".inr"
	electrodes="./inputs/"$mesh_name".electrodes"
	parameters="./inputs/input_idx.txt"
	output_dir="./output/"

	VM_share_folder="/mnt/test_share/output_meshes/"

	#Clear pervious outputs
	rm ./output/*

	./bin/mesher -i $input -e $electrodes -p $parameters -d $output_dir -o $mesh_name

	# Using a VM, so copy the paraview files to the host pc
	echo "Copying Paraview files to host PC"
	cp $output_dir* $VM_share_folder
done
