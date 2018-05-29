# Mesher
## Build instructions (tested on Ubuntu 16 & 17)

* Install dependencies
```
sudo apt-get install cmake libcgal-dev libcgal-qt5-dev libglu1-mesa libxi-dev libxmu-dev libglu1-mesa-dev
```

* Clone repository
```
git clone https://github.com/EIT-team/Mesher.git
```

* Make build & output directories
```
cd Mesher
mkdir build
mkdir output
```
* Compile
```
cd build
cmake ..
make
```

* Test
```
cd ../test/
./run_tests
```

* Run mesher ( from /Mesher root directory)
```
Usage:
./bin/mesher -i INPUT_INR -e INPUT_ELECTRODES -p INPUT_PARAMETERS
        -i input image file
        -e electrode position file
        -p parameter file
        -o output mesh name (default = new_mesh)
        -d output directory (default = output/)
        
Example:

./bin/mesher -i inputs/input.inr -e inputs/Electrodes.txt -p inputs/input_idx.txt
```
