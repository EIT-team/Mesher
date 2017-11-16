
#ifndef WARP_H
#define WARP_H

#include "stretch_info.h"
#include "CGAL_include.h" // for Image_3

#include <vector>
#include <unordered_set>
#include <climits>
#include <math.h>
using namespace std;

class Deform_Volume {

public:

  unsigned char * image_data;
  int dims;
  double vx, vy, vz; // Size of a voxel along each axis

  // max/min bounds of object in volume
  int xmax, ymax, zmax;
  int xmin, ymin, zmin;
  int xmid, ymid, zmid; // Centre of each dimensions

  vector<int> layers;

  // Default values, can be changed in mesh input/parameter file
  int min_stretch = 5;
  int max_stretch = 50;

// For stretching
int point_to_move, distance_to_move, anchor;

  // Store the parameters of the deformation
  string deformation_info;

  Deform_Volume();
  Deform_Volume(CGAL::Image_3*);

  //Deform_Volume(CGAL::Image_3);
  void dilate_layer( int, int);
  void stretch_array(vector<int>);

  bool check_valid_points();
  void modify_image();

  void random_stretch();
  void random_dilate();

  void defined_stretch(std::vector<double>);

  int random_stretch_point(int idx_min, int idx_max);
  int random_anchor_point(int idx_min, int idx_max);

  long get_array_index(int x, int y, int z);
  vector<long> neighbouring_elements(long voxel_index);
  void get_layers();
  void find_mesh_bounds();


};

#endif
