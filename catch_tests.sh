set -e # Terminate script on any error

INCLUDE_DIR=src/

g++ test/run_tests.cpp test/test_warping.cpp test/test_stretching.cpp src/stretch_info.cpp src/warp_mesh.cpp -I$INCLUDE_DIR

./a.out
