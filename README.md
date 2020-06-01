[![Build Status](https://travis-ci.org/EIT-team/Mesher.svg?branch=master)](https://travis-ci.org/EIT-team/Mesher)
# EIT-Mesher

EIT-MESHER is C++ software, based on the CGAL library, which generates high quality Finite Element Model tetrahedral meshes from binary masks of 3D volume segmentations. Originally developed for biomedical applications in Electrical Impedance Tomography (EIT) to address the need for custom, non-linear refinement in certain areas (e.g. around electrodes), EIT-MESHER can also be used in other fields where custom FEM refinement is required, such as Diffuse Optical Tomography (DOT).

<img src = "./examples/brain/figures/brain_PV_HR.png" alt="Rat Brain Mesh" width = 600>

[Example Usage](./examples/readme.md)  
[Unit Cube - 'Hello World' example](./examples/unitcube/readme.md)  
[Meshing segmented brain data](./examples/brain/readme.md)  
[Meshing from STL file](./examples/neonatescalp/readme.md)  
[Mesh refinement examples](./examples/refinements/readme.md)  
[Mesh deformation](./examples/deformation/readme.md)

## Using the Meshes

Examples for using the EIT-MESHER outputs with common EIT and DOT solvers are given in [solvers examples](examples/solvers)

## Build instructions (tested on Ubuntu 16, 17, 18)

* Install dependencies

```bash
sudo apt-get install build-essential cmake libcgal-dev libcgal-qt5-dev libglu1-mesa libxi-dev libxmu-dev libglu1-mesa-dev
```

* Clone repository

```bash
git clone https://github.com/EIT-team/Mesher.git
```

* Make build & output directories

```bash
cd Mesher
mkdir build
mkdir output
```

* Compile

```bash
cd build
cmake ..
make
```

* Test

```bash
cd ../test/
./run_tests
```

* Run mesher ( from /Mesher root directory)

```bash
Usage:
./bin/mesher -i INPUT_INR -e INPUT_ELECTRODES -p INPUT_PARAMETERS
        -i input image file
        -e electrode position file
        -p parameter file
        -o output mesh name (default = new_mesh)
        -d output directory (default = output/)
```

Example:

```bash
./bin/mesher -i inputs/input.inr -e inputs/Electrodes.txt -p inputs/params.txt
```

Produces the following mesh as viewed in paraview

![Brain Example](examples/brain/figures/brain_PV.png)

## Docker instructions (for Ubuntu 19&20+)

EIT-MESHER can be built in a container to install on newer linux installations. Windows users require WSL2 for docker to work.

### Install container

The docker container can be obtained using two different methods:

* Build container from source (`mesher` is a name of your choice):

```bash
docker build -t mesher .
```

* Pull image directly from dockerhub without having to build

```bash
docker pull doctorjimbles/eit-mesher
```

### Running Mesher from container

To test it is working run the container with no inputs, this will call the mesher with defaults which runs the single example included in the container:

```bash
docker run --rm mesher
```

The `--rm` flag removes the container after its done to prevent having lots of unnecessary container IDs.

The mesher can then be called with the parameter arguments as normal **Note** these are files *inside* the container.

```bash
docker run --rm mesher -i inputs/input.inr -e inputs/Electrodes.txt -p inputs/params.txt
```

* Using your own data - mounting volumes

The container has two volumes `/input` and `/output` which can be mounted to a directory on the host using the `-v` flag. So for example with the `MESHER` repository on the home dir:

```bash
docker run --rm -v ~/Mesher/inputs:/input -v ~/Mesher/output:/output  mesher
```

The mesher can then finally be called by combining the volume definitions and the parameters. **Note** The directories must be with respect to the container file structure

```bash
docker run --rm  -v ~/Mesher/inputs:/input -v ~/Mesher/output:/output  mesher -i /input/input.inr -e /input/Electrodes.txt -p /input/params.txt -d /output/ -o dockertest
```
