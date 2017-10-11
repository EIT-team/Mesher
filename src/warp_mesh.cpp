
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
    int point_to_move, int distance_to_move, int anchor, char direction, int dims ) {

      /*    'Stretch' array contents

      image_data: input array
      point_to_move: array index of point to move
      distance_to_move: how many elemnts to move this point by
      anchor: fixed point, relative to which 'stretching' is calcualted
      direction: axis along which to stretch (x,y,z)

      */


      cout << "Stretching array" << endl << "Point: " << point_to_move << " Distance: "
      << distance_to_move << " Anchor: " << anchor << " along dimension: " << direction << endl;
      // Check valid point is given
      if (point_to_move == anchor) {
        cout << "Point to move cannot be the same as the anchor point" << endl;
        return;
      }

      // TODO: This won't catch the case where  point_to_move < anchor
      // Check that the new point location(s) will be within the array bounds
      if ((point_to_move + distance_to_move) > dims) {
        cout << " Distance to move too great, new point outside bounds of array" << endl;
        return;
      }


      int start_iterate, end_iterate;
      int move_point_dist_from_anchor;
      int stretch_scale;

      // TODO: Better way to handle this?
      // /Is point to move to the left of anchor
      if (point_to_move < anchor) {
        start_iterate  = point_to_move - distance_to_move;
        move_point_dist_from_anchor = point_to_move - distance_to_move - anchor;
        end_iterate = anchor - 1;
        stretch_scale = 1;

      }

      else {
        start_iterate = point_to_move + distance_to_move;
        move_point_dist_from_anchor = point_to_move + distance_to_move - anchor;
        end_iterate = anchor + 1;
        stretch_scale = -1;
      }

      int i, j, k;
      int this_idx, vec_to_copy_from, idx_to_copy_from;
      int distance_from_anchor;
      double distance_anchor_ratio, stretch_ratio;

      for (i = start_iterate; i != end_iterate; i += stretch_scale) {

        distance_from_anchor = i - anchor;
        distance_anchor_ratio = distance_from_anchor / (double)move_point_dist_from_anchor;
        stretch_ratio = stretch_scale * distance_anchor_ratio * distance_anchor_ratio;
        vec_to_copy_from = i + stretch_ratio * distance_to_move;

        for (j = 0; j < dims; j++) {
          for (k = 0; k < dims; k++) {

            // TODO: better way to handle different directions?
            if (direction == 'y') {
              this_idx = get_array_index( i, k, j, dims);
              idx_to_copy_from  = get_array_index(vec_to_copy_from, k, j, dims);
            }

            if (direction == 'z') {
              this_idx = get_array_index( k, i, j, dims);
              idx_to_copy_from  = get_array_index(k, vec_to_copy_from, j, dims);
            }

            if (direction == 'x') {
            this_idx = get_array_index( j, k, i, dims);
            idx_to_copy_from  = get_array_index(j, k, vec_to_copy_from, dims);
            }

            image_data[this_idx] = image_data[idx_to_copy_from];
          }
        }

      }

    }

    void modify_image(unsigned char * image_data, int dims) {

      std::cout << std::endl << "MODIFYING IMAGE DATA" << endl;

      //TODO: Check that input inr file actually has a domain/tissue type assigned.
      // TODO: Do some actual stetching of mesh
      // TODO:  pass dims to this function
      //stretch_array_1D( image_data, 60, 25, 128, 'x', 256);
     //dilate_layer(image_data, 5, 1, 256);

     srand(time(NULL));

     // TODO: write parameters of random deformations to some file
     // Do at least one
     random_stretch(image_data, dims);

     while (rand() % 2) {
     random_stretch(image_data, dims);


}
    // 50% chance of dilation
     if (rand() %2) {
      ;// random_dilate(image_data, dims);
     }

}

void random_stretch(unsigned char* image_data, int dims) {

  char directions[3] = {'x', 'y', 'z'};
  char rand_direction = directions [ rand() % 3 ];

  // Try to generate 'reasonable' maniuplation points
  int anchor = (dims / 4 ) + rand() % (dims / 4 ); // In middle

  int lower_quarter = rand() % (dims/6); // 0 to 1/4
  int upper_quarter = (5*dims/6) + lower_quarter; // 3/4 to endl
  int stretch_point;

  if (rand() %2) {
  stretch_point = lower_quarter;
}
else {
  stretch_point = upper_quarter;

}

  // Random distance, no bigger than max_stretch and remaining within the bounds of array
  int max_stretch = 25;
  // TODO: doesn't account for
  int distance = rand() % (min (max_stretch, min(stretch_point, dims - stretch_point)));

  stretch_array_1D(image_data, stretch_point, distance, anchor, rand_direction, dims);
}

void random_dilate(unsigned char* image_data, int dims) {
  // Dilate a random layer by a random amount (between  1 and 3 pixels)

  int layer, dilate_amount;
  int max_dilate = 1;

  // Get list of layer indices and select one at random
  vector<int> layers = get_layers(image_data, dims);
  int random_index = rand() % layers.size();
  layer = layers[random_index];

  dilate_amount  =1 + rand() % max_dilate;

dilate_layer(image_data, layer, dilate_amount, dims);

}

vector<int> get_layers(unsigned char* image_data, int dims) {
// Placeholder for function to loop through array and return a vector of all the unique values
  vector<int> layers;

  for (int i = 1; i <= 7; i++) {
    layers.push_back(i);
  }

  return layers;
}
