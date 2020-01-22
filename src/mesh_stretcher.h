#ifndef mesh_stretcher_H
#define mesh_stretcher_H

#include <iostream>
#include <math.h> //for round

/** Class to carry out stretching operation on mesh. **/
class Mesh_Stretcher
{
public:
  bool active; // Is stretching happening in this dimension?
  int point_to_move, distance, anchor;
  int start_iterate, end_iterate, step;
  int dims;

  int move_point_dist_from_anchor, distance_from_anchor;
  double distance_anchor_ratio, stretch_ratio;

  Mesh_Stretcher(int, int, int, int);
  void prepare_stretch(int);
  int idx_to_copy_from(int);
  bool check_valid_points();
  std::string stretch_description();
};

#endif
