#include "mesh_stretcher.h"

using namespace std;

Mesh_Stretcher::Mesh_Stretcher(int point, int dist, int anch, int dim)
{

  point_to_move = point;
  distance = dist;
  anchor = anch;
  dims = dim;

  // Check if we should do a stretch, exit if not
  if (!check_valid_points())
  {
    active = false;
    start_iterate = 0;
    end_iterate = dims;
    step = 1;

    return;
  }

  active = true;

  // Move point to left of anchor
  if (point_to_move < anchor)
  {
    start_iterate = point_to_move - distance;
    move_point_dist_from_anchor = point_to_move - distance - anchor;
    end_iterate = anchor - 1;
    step = 1;
  }

  else
  {
    start_iterate = point_to_move + distance;
    move_point_dist_from_anchor = point_to_move + distance - anchor;
    end_iterate = anchor + 1;
    step = -1;
  }
}
/** Chek that the stretching parameters are valid.
 **/
bool Mesh_Stretcher::check_valid_points()
{

  // A negative value means no stretch will be performed
  if (point_to_move < 0)
  {
    return false;
  }

  // Move point is the same as anchor point
  if (point_to_move == anchor)
  {
    cout << "Move point the same as anchor point, setting to 0." << endl;
    return false;
  }

  //The new move point will end up outside the bounds of array
  if ((point_to_move > anchor) && (point_to_move + distance) > dims)
  {
    cout << "New points will be outside bounds of array (too big), setting to 0." << endl;
    return false;
  }

  // Remaingin case,  point to move must be < anchor
  if ((point_to_move - distance) < 0)
  {
    cout << "New points will be outside bounds of array (too small), setting to 0." << endl;
    return false;
  }

  return true;
}

/** Calculate the stretch parameters for a given index, i
 * If not stretching in this direction, need to set stretch_ratio to 1
 * so that this dimension is effectively ignored i.e. it doesn't affect the
 * min(stretch_ratios) calculation
 **/
void Mesh_Stretcher::prepare_stretch(int i)
{

  if (!active)
  {

    stretch_ratio = 1;
    return;
  }

  distance_from_anchor = i - anchor;
  distance_anchor_ratio = distance_from_anchor / double(move_point_dist_from_anchor);
  stretch_ratio = distance_anchor_ratio * distance_anchor_ratio;
}

/** Calculate the array index from which to copy, this is essentailly the 
   * 'stretching' part of the algorithm.
   **/
int Mesh_Stretcher::idx_to_copy_from(int i)
{

  //if not doing any stretching in this dimension, return the same index
  if (!active)
  {
    return i;
  }

  // Make sure that elements outside of the desired range aren't accessed.
  // If they are, just return the same index
  if ((step > 0) && ((i < start_iterate) || (i > end_iterate)))
  {
    cout << "Trying to stretch point outside of defined range" << endl;
    return i;
  }

  if ((step < 0) && ((i > start_iterate) || (i < end_iterate)))
  {
    cout << "Trying to stretch point outside of defined range" << endl;
    return i;
  }

  int idx_to_copy = i + round(step * stretch_ratio * distance);

  return idx_to_copy;
}

/** Return a string which gives details of the stretch which has been performed.
 **/
string Mesh_Stretcher::stretch_description()
{

  if (!active)
  {
    return "No stretch\n";
  }

  return "Point to move: " + to_string(point_to_move)
         + " Distance to move: " + to_string(distance)
         + " Anchor point: " + to_string(anchor) + "\n";
}
