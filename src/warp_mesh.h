
#ifndef WARP_H
#define WARP_H

#include "include.h"

using namespace std;

long get_array_index(int,int,int, int);
void modify_image(unsigned char *, int) ;
void stretch_array_1D( unsigned char *,  int, int, int, char, int );
void dilate_layer(unsigned char *, int, int,  int);
void change_neighbours(unsigned char *, long, int);
vector<long> neighbouring_elements(long,int);
vector<int> find_mesh_bounds(unsigned char *, int);


void random_stretch(unsigned char*, int);
void random_dilate(unsigned char*, int);
vector<int> get_layers(unsigned char*, int);

class Deform_Volume {

public:

  unsigned char * image_data;
  int dims;

  // max/min bounds of object in volume
  int xmax, ymax, zmax;
  int xmin, ymin, zmin;

  vector<int> layers;

  Deform_Volume();
  Deform_Volume(void *, int);

  //Deform_Volume(CGAL::Image_3);
  void dilate_layer( int layer, int n_pixels);
  void stretch_array_1D( int point_to_move, int distance_to_move, int anchor, char direction);
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


};

#endif
