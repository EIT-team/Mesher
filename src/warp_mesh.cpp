
#include "warp_mesh.h"

long get_array_index(int x, int y, int z, int dims) {
  /* Image data is represented (to humans) as 3D voxel data e.g. array[x][y][z]. To CGAL,
   it is just a one dimensional array which contains all of the voxels.

   Return the one dimensinoal array location corresponding to a
   particular voxel in the image.
*/

  long idx;

  idx = (x * dims + y) * dims + z;

  if (idx > dims*dims*dims) {
    std::cout << "Array index too big!";
    return -1;
  }

  if (x < 0 || y < 0 || z < 0) {
    std:cout << "Negative value passed!";
    return -1;
  }

  return idx;

}

void modify_image(unsigned char * image_data) {

  std::cout << std::endl << "MODIFYING IMAGE DATA" << endl;

  //TODO: Check that input inr file actually has a domain/tissue type assigned.
  // TODO: Do some actual stetching of mesh

  //cout << "Xdim: " << image.xdim() << "  Ydim: " << image.ydim() << "  Zdim: " << image.zdim() << endl;
  //cout << "vX: " << image.vx() << "  vY: " << image.vy() << " vZ: " << image.vz() << endl;

  int x,y,z;
  int dim = 256;

  // Voxel data is stored as unsigned char by default
  // Casting it to unsigned int is more useful/readable for processing
  unsigned char this_voxel_char;
  unsigned int this_voxel_int;

  for (long i = 0; i < dim*dim*dim; i++) {
    this_voxel_int = (unsigned int)image_data[i];

    // Change anything that isnt't background (index 0) to the same index
    if (this_voxel_int != 0) {
      image_data[i] = (unsigned char)(7);
    }

  }

  }
