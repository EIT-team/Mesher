
#ifndef WARP_H
#define WARP_H

#include "stretch_info.h"

#include <vector>
#include <unordered_set>
#include <climits>
#include <math.h>

using namespace std;

class Deform_Volume {

public:

  unsigned char * image_data;
  int dims;

  // max/min bounds of object in volume
  int xmax, ymax, zmax;
  int xmin, ymin, zmin;

  vector<int> layers;

  // Default values, can be changed in mesh input/parameter file
  int min_stretch = 5;
  int max_stretch = 50;

// For stretching
int point_to_move, distance_to_move, anchor;

  // Store the parameters of the deformation
  string deformation_info;

  Deform_Volume();
  Deform_Volume(void *, int);

  //Deform_Volume(CGAL::Image_3);
  void dilate_layer( int, int);
  void stretch_array_1D(char);
  void stretch_array(vector<int>);

  bool check_valid_points();
  void modify_image();

  void random_stretch();
  void random_dilate();
  int random_stretch_point(int idx_min, int idx_max);
  int random_anchor_point(int idx_min, int idx_max);

//private:
  long get_array_index(int x, int y, int z);
  vector<long> neighbouring_elements(long voxel_index);
  void get_layers();
  void find_mesh_bounds();

//TODO: Functions to centre data in array and to pad array with 0s, to give more
//space to deform into

};



#endif
