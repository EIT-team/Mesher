
#include "warp_mesh.h"

long get_array_index(int x, int y, int z, int dims) {
  /* Image data is represented (to humans) as 3D voxel data e.g. array[x][y][z]. To CGAL,
  it is just a one dimensional array which contains all of the voxels.

  Return the one dimensinoal array location corresponding to a
  particular voxel in the image.
  */

  long idx;

  idx = (x * dims + y) * dims + z;

  if (idx >= dims*dims*dims) {
    //std::cout << "Array index too big!" << std::endl;
    return -1;
  }

  if (x < 0 || y < 0 || z < 0) {
    //std:cout << "Negative value passed!" << std::endl;
    return -1;
  }

  return idx;

}


void dilate_layer(unsigned char * image_data, int layer_index, int n_pixels, int dims) {
  /* Expands one a particular layer/tissue by one voxel on all sides
  image_data: input array
  layer_index: the layer/tissue index to be dilated
  n_pixels: how many pixels/voxels to dilate the layer by
  dims: length of the x/y/z dimension in image_data

  */
 cout << "Dilating layer " + to_string(layer_index) + " by " + to_string(n_pixels) + " pixels" <<endl;
  unsigned char layer_index_char = (unsigned char)layer_index;

  long i;
  int n_elements = dims * dims * dims;
  vector<long> elements_to_change, neighbours;
  long vector_element;

// Loop for each level of pixel dilation
  while (n_pixels--) {
  for (i = 0; i <  n_elements; i++) {
    if (image_data[i] == layer_index_char) {

      // Get the neighbouring elements
      neighbours = neighbouring_elements( i, dims);

      // Add these to the list of elments to change
      elements_to_change.insert(
        elements_to_change.end(), neighbours.begin(), neighbours.end());


      }

    }

    // Do the dilation
    // Go through the elements and change the layer type
    while (!elements_to_change.empty()) {

      vector_element = elements_to_change.back();
      image_data[vector_element] = layer_index_char;
      elements_to_change.pop_back();

    }
  }

  }

  /*void change_neighbours(unsigned char * image_data, long idx, int dims) {

  unsigned char layer_index = image_data[idx];

  // Get list of neighbouring elements
  vector<long> neighbours = neighbouring_elements( idx, dims);

  long vector_element;

  // Do the dilation
  // Go through the elements and change the layer type
  while (!neighbours.empty()) {

  vector_element = neighbours.back();
  image_data[vector_element] = layer_index;
  neighbours.pop_back();

}
}*/


vector<long> neighbouring_elements (long voxel_index, int dims) {
  /* Calculate the indexes of elements neighbouring a partiuclar voxel in 3D array
  */

  int x,y,z; //Indexes of voxel

  z = voxel_index % dims;
  y = ( ( voxel_index - z ) / dims ) % dims;
  x = ( ( voxel_index - z) - y * dims) / (dims*dims);

  vector<long> neighbours;

  int i,j,k;
  long this_idx;

  for (i = x - 1; i <= x + 1; i++) {
    for (j = y - 1; j <= y + 1; j++) {
      for (k = z - 1; k <= z + 1; k++) {

        this_idx = get_array_index(i, j, k, dims);

        // Check that voxel is within bounds [0 NDIMS] and that it is not the original voxel
        if (i >= 0 && i < dims &&  j >= 0 && j < dims && k >= 0 && k < dims &&
          this_idx != voxel_index) {
            neighbours.push_back(this_idx);
          }
        }
      }
    }

    return neighbours;

  }

  void stretch_array_1D( unsigned char * image_data,
    int point_to_move, int distance_to_move, int anchor, int dims ) {

      /*    'Stretch' array contents

      image_data: input array
      point_to_move: array index of point to move
      distance_to_move: how many elemnts to move this point by
      anchor: fixed point, relative to which 'stretching' is calcualted

      */


      // Check valid point is given
      if (point_to_move == anchor) {
        cout << "Point to move cannot be the same as the anchor point" << endl;
        return;
      }

      int start_iterate, end_iterate;
      int move_point_dist_from_anchor;
      int stretch_scale;


      // /Is point to move to the left of anchor
      if (point_to_move < anchor) {
        start_iterate  = point_to_move - distance_to_move;
        end_iterate = anchor - 1;
        stretch_scale = 1;

      }

      else {
        start_iterate = point_to_move + distance_to_move;
        end_iterate = anchor + 1;
        stretch_scale = -1;
      }

      int i, j, k;
      int this_idx, vec_to_copy_from, idx_to_copy_from;

      for (i = start_iterate; i != end_iterate; i += stretch_scale) {
        vec_to_copy_from = i + stretch_scale * distance_to_move;

        for (j = 0; j < dims; j++) {
          for (k = 0; k < dims; k++) {
            this_idx = get_array_index( i, j, k, dims);
            idx_to_copy_from  = get_array_index( vec_to_copy_from, j, k, dims);

            image_data[this_idx] = image_data[idx_to_copy_from];
          }
        }

      }

    }

    void modify_image(unsigned char * image_data) {

      std::cout << std::endl << "MODIFYING IMAGE DATA" << endl;

      //TODO: Check that input inr file actually has a domain/tissue type assigned.
      // TODO: Do some actual stetching of mesh

      //stretch_array_1D( image_data, 225, 5, 128, 256);
     dilate_layer(image_data, 5, 1, 256);


      /*
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
  */
}
