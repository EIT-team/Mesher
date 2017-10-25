#ifndef STRTECH_INFO_H
#define STRETCH_INFO_H

#include <iostream>

class Stretch_Info {
public:

  bool active; // Is stretching happening in this dimension?
  int point_to_move, distance, anchor;
  int start_iterate, end_iterate, step;
  int dims;

  int move_point_dist_from_anchor, distance_from_anchor;
  double distance_anchor_ratio, stretch_ratio;

  Stretch_Info(int, int, int, int);
  int idx_to_copy_from(int);
  bool check_valid_points();
  std::string stretch_description();
};

#endif
