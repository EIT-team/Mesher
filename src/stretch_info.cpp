#include "stretch_info.h"
#include <iostream>

using namespace std;

Stretch_Info::Stretch_Info(int point, int dist, int anch) {

//TODO: tidy this up, shouldn't need to reallcoate straight away
point_to_move = point;
distance = dist;
anchor = anch;

// Less than 0 indicates this dimension isn't being stretched,
// so set
if (point_to_move < 0) {
  active = false;
  start_iterate = 0;
  end_iterate = 512; // TODO: dims
  step = 1;


  return;
}

  active = true;

  // Move point to left of anchor
  if (point_to_move < anchor) {
    start_iterate  = point_to_move - distance;
    move_point_dist_from_anchor = point_to_move - distance - anchor;
    end_iterate = anchor - 1;
    step = 1;

  }

  else {
    start_iterate = point_to_move + distance;
    move_point_dist_from_anchor = point_to_move + distance - anchor;
    end_iterate = anchor + 1;
    step = -1;
  }

}

int Stretch_Info::idx_to_copy_from( int i) {

  //if not doing any stretching in this dimension, return the same index
  if (!active) {
    return i;
  }

  //cout << i << endl;
  distance_from_anchor = i - anchor;
  distance_anchor_ratio = distance_from_anchor / double(move_point_dist_from_anchor);
  stretch_ratio = step * distance_anchor_ratio * distance_anchor_ratio;

  int idx_to_copy = i + stretch_ratio * distance;
  return idx_to_copy;

}

string Stretch_Info::stretch_description() {

  if (!active) {
    return "0.0.0";
  }

  return to_string(point_to_move) + "." + to_string(distance) + "." + to_string(anchor);

}
