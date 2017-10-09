set -e # Terminate script on any error
input="./inputs/reduced_elements_01000.inr"
electrodes="./inputs/Electrodes.txt"
parameters="./inputs/input_idx.txt"
output_dir="./output/"
output_name="test_out"

VM_share_folder="/mnt/test_share/"

./bin/mesher_rat -i $input -e $electrodes -p $parameters -d $output_dir -o $output_name

# Using a VM, so copy the paraview files to the host pc
echo "Copying Paraview files to host PC"
cp $output_dir*.vtu $VM_share_folder
