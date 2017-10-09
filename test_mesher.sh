# Run mesher with known input mesh of rat brain
# Check it is working OK by comparing the dgf file hash against its expected value
# Not an exhaustive test, but works well enough for now

set -e # Terminate script on any error
input="./inputs/input.inr"
electrodes="./inputs/Electrodes.txt"
parameters="./inputs/input_idx.txt"
output_dir="./output/"
output_name="test_out"

VM_share_folder="/mnt/test_share/"

./bin/mesher_rat -i $input -e $electrodes -p $parameters -d $output_dir -o $output_name

expected_md5_pre_opt="023c9d3cec2b5c55a7f833050f9e0325"
expected_md5_final="ce46764edc9c6e8583cbb5128f4412ae"

#md5sum gives two outputs, only want to save the first one
dgf_md5_pre_opt=$(md5sum $output_dir$output_name.dgf.pre_optimise | awk '{print $1}')
dgf_md5_final=$(md5sum $output_dir$output_name.dgf | awk '{print $1}')

echo $expected_md5
echo $output_dgf_md5

echo "Testing non-optimised mesh hash:"
if [ "$expected_md5_pre_opt" = "$dgf_md5_pre_opt" ]; then
  echo "Hashes match"
else
  echo "Hashes don't match - failure"
fi


echo "Testing optimised mesh hash:"
if [ "$expected_md5_final" = "$dgf_md5_final" ]; then
  echo "Hashes match"
else
  echo "Hashes don't match - failure"
fi
