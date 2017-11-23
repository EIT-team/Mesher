
#include "warp_mesh.h"

// Warper Class
Deform_Volume::Deform_Volume() {
  ;
}

Deform_Volume::Deform_Volume(CGAL::Image_3 *image) {

  // Default values, can be changed in mesh input/parameter file
  min_stretch = 5;
  max_stretch = 50;

  image_data = (unsigned char*)image->data();
  dims = image->xdim();

  vx =  image->vx();
  vy = image->vy();
  vz = image->vz();

  find_mesh_bounds();

  deformation_info = "";
}

long Deform_Volume::get_array_index(int x, int y, int z) {
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


void Deform_Volume::dilate_layer(int layer_index, int n_pixels) {
  /* Expands one a particular layer/tissue by one voxel on all sides
  layer_index: the layer/tissue index to be dilated
  n_pixels: how many pixels/voxels to dilate the layer by
  */
  cout << "Dilating layer " + to_string(layer_index) + " by " + to_string(n_pixels) + " pixels" <<endl;

  unsigned char layer_index_char = (unsigned char)layer_index;

  long n_elements = dims * dims * dims;
  long vector_element, i;
  vector<long> neighbours;
  unordered_set<long> elements_to_change;

  deformation_info += "_d." + to_string(layer_index) + "." + to_string(n_pixels);

  // Loop for each level of pixel dilation
  while (n_pixels--) {
    for (i = 0; i <  n_elements; i++) {
      if (image_data[i] == layer_index_char) {
        // Get the neighbouring elements
        neighbours = neighbouring_elements(i);

        // Add all neighbours to the list of elements to change
        while (!neighbours.empty()) {

          vector_element = neighbours.back();
          neighbours.pop_back();
          elements_to_change.insert(vector_element);

        }
      }
    }

    // Do the dilation
    // Go through the elements and change the layer type
    unordered_set<long>::iterator itr;
    for (itr = elements_to_change.begin(); itr != elements_to_change.end(); itr++) {
      image_data[*itr] = layer_index_char;
    }

  }

}

vector<long> Deform_Volume::neighbouring_elements (long voxel_index) {
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

        this_idx = get_array_index(i, j, k);

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

  bool Deform_Volume::check_valid_points() {
    // Check that points are avalid and that new point will be within the the bounds of the array

    if (point_to_move == anchor) {
      cout << "Point to move cannot be the same as the anchor point" << endl;
      return false;
    }

    bool too_big = (point_to_move + distance_to_move) > dims;
    bool too_small = (point_to_move - distance_to_move) < 0;

    if (too_big || too_small) {
      cout << " Distance to move too great, new point outside bounds of array" << endl;
      return false;
    }

    return true;

  }

  void Deform_Volume::defined_stretch(vector<double> distance_mm) {
    /* Deform the mesh by a particular amount (disance_mm) in a particular x/y/z direction
    If distance_mm[x/y/z] is 0, no stretch is performed in that direction.
    A positive/negative value stretches to the right/left respectively.
    */
    int n_dims = distance_mm.size();
    // Check inputs, should be 3 elments, one for each dimension
    if (n_dims != 3) {

      cout << "Invalid number of values passed, should be 3, one for each dimension" << endl;

    }
//TODO: Tidy up here, too messy/repeated

    vector<int> stretch;
    int point, anchor, voxel_distance;

    //x data
    double xdist = distance_mm[0];

    if (xdist> 0) point = xmax; //stretch right
    else if (xdist < 0) point = xmin; //stretcg left
    else point = -1; //Do nothing

    anchor = xmid;
    // Covert distance in mm to number of voxels, basxed on the distance for each voxel.
    voxel_distance = round (abs( xdist / vx));

    stretch.push_back(point);
    stretch.push_back(voxel_distance);
    stretch.push_back(anchor);

    //y data
    double ydist = distance_mm[1];

    if (ydist> 0) point = ymax; //stretch right
    else if (ydist < 0) point = ymin; //stretcg left
    else point = -1; //Do nothing

    anchor = ymid;
    voxel_distance = round ( abs(ydist) / vy);

    stretch.push_back(point);
    stretch.push_back(voxel_distance);
    stretch.push_back(anchor);

    //z data
    double zdist = distance_mm[2];

    if (zdist> 0) point = zmax; //stretch right
    else if (zdist < 0) point = zmin; //stretcg left
    else point = -1; //Do nothing

    anchor = zmid;
    voxel_distance = round ( abs(zdist) / vz);

    stretch.push_back(point);
    stretch.push_back(voxel_distance);
    stretch.push_back(anchor);

    // Do the stretch
    stretch_array(stretch);

  }


  void Deform_Volume::stretch_array(vector<int> stretch) {
    /* Stretch the image
    vector<int> stretch: 9 element vector (3 for x, 3 for y, 3 for z) that gives the paremetners
    of the stretch to be performed. For each dimension:
      1st element - point at which stretch starts
      2nd element - distance this point will be moved
      3rd element - anchor point. The stretching distance for each elment is relative
      to its distance from this point.

      Only points between the move point and the anchor point are stretched.

    */
    // Print stretch parameters
    cout << "Stretching with parameters:" << endl;
    int vec_i;

    for( vec_i = 0; vec_i < stretch.size(); ++vec_i) {
      cout << stretch[vec_i] << " ";
    }
    cout << endl;

    int this_idx, idx_to_copy_from;
    int new_x, new_y, new_z;

    double min_stretch_ratio;

    // Create stretch objects in each dimension
    Stretch_Info x(stretch[0],stretch[1],stretch[2], dims);
    Stretch_Info y(stretch[3],stretch[4],stretch[5], dims);
    Stretch_Info z(stretch[6],stretch[7],stretch[8], dims);

    // Loop over each element that is affected by the stretch
    int x_idx, y_idx, z_idx;
    for (x_idx = x.start_iterate; x_idx != x.end_iterate; x_idx += x.step) {
      for (y_idx = y.start_iterate; y_idx != y.end_iterate; y_idx += y.step) {
        for (z_idx = z.start_iterate; z_idx != z.end_iterate; z_idx += z.step) {

          x.prepare_stretch(x_idx);
          y.prepare_stretch(y_idx);
          z.prepare_stretch(z_idx);

          //TODO: Explain this bit here!
          min_stretch_ratio = min ( abs(x.stretch_ratio), abs(y.stretch_ratio));
          min_stretch_ratio = min( min_stretch_ratio, abs(z.stretch_ratio));

          x.stretch_ratio = min_stretch_ratio;
          y.stretch_ratio = min_stretch_ratio;
          z.stretch_ratio = min_stretch_ratio;

          // Calculate where to copy from
          new_x = x.idx_to_copy_from(x_idx);
          new_y = y.idx_to_copy_from(y_idx);
          new_z = z.idx_to_copy_from(z_idx);

          // Copy point over
          this_idx = get_array_index(z_idx,y_idx,x_idx);
          idx_to_copy_from = get_array_index(new_z, new_y, new_x);
          image_data[this_idx] = image_data[idx_to_copy_from];

        }
      }
    }

    deformation_info += "_s";
    deformation_info += x.stretch_description() + ".";
    deformation_info += y.stretch_description() + ".";
    deformation_info += z.stretch_description();


  }

  void Deform_Volume::modify_image() {

    std::cout << std::endl << "MODIFYING IMAGE DATA" << endl;

    string stretch_info = "";

    srand(time(NULL));

    // Do at least one deformation
    random_stretch();

    // 50% chance of cotinuing
     while (rand() % 2); {

      //50% chance of stretch
      if (rand() %2) {
      random_stretch();

    }

    // 1 in 3 % chance of dilation
    if ((rand() %3) < 1) {
      random_dilate();
    }

    find_mesh_bounds(); //Update edges of object

    }


}


  void Deform_Volume::random_stretch() {

    vector<int> stretch;

    //TODO: point_to_move, distance and anchor are class variables, is this needed anymore?
    //x data
    point_to_move = random_stretch_point(xmin, xmax);
    anchor = random_anchor_point(xmin, xmax);
    distance_to_move = min_stretch + rand() % (min (max_stretch, min(point_to_move, dims - point_to_move)));

    stretch.push_back(point_to_move);
    stretch.push_back(distance_to_move);
    stretch.push_back(anchor);

    //y data
    point_to_move = random_stretch_point(ymin, ymax);
    anchor = random_anchor_point(ymin, ymax);
    distance_to_move = min_stretch + rand() % (min (max_stretch, min(point_to_move, dims - point_to_move)));

    stretch.push_back(point_to_move);
    stretch.push_back(distance_to_move);
    stretch.push_back(anchor);

    //z data
    point_to_move = random_stretch_point(zmin, zmax);
    anchor = random_anchor_point(zmin, zmax);
    distance_to_move = min_stretch + rand() % (min (max_stretch, min(point_to_move, dims - point_to_move)));

    stretch.push_back(point_to_move);
    stretch.push_back(distance_to_move);
    stretch.push_back(anchor);

    // Randomly turn on or off stretchig in each dimension
    //TODO: this can be better!
    if (!(rand() % 4)) {
      stretch[0] = -1; //x
    }

    if (!(rand() % 4)) {
      stretch[3] = -1; //y
    }

    if (!(rand() % 4)) {
      stretch[6] = -1; //z
    }

    stretch_array(stretch);
  }


  int Deform_Volume::random_stretch_point(int idx_min, int idx_max) {
    // Return a stretch point that is < idx_min or > idx_max

    // Pick  a point 1 voxels away from the maximum/minimum
    // Check it is not outside of the array (> dims or < 0)
    int upper = idx_max + 1;
    int lower = idx_min - 1;

    // Make sure they are within bounds of array
    upper = min(upper, dims);
    lower = max(lower, 0);

    if (rand() %2) {
      return upper;
    }

    return lower;

  }


  int Deform_Volume::random_anchor_point(int idx_min, int idx_max) {
    // Want to pick an anchor point  'within' the object
    // i.e. >idx_min and < idx_max
    return idx_min + rand() % ( idx_max - idx_min );

  }

  void Deform_Volume::random_dilate() {
    // Dilate a random layer by a random amount (either  1 or 2 pixels)

    int layer, dilate_amount;
    int max_dilate = 1;

    // Get list of layer indices and select one at random
    get_layers();
    int random_index = rand() % layers.size();
    layer = layers[random_index];

    dilate_amount  =1 + rand() % max_dilate;

    dilate_layer(layer, dilate_amount);

  }


  void Deform_Volume::get_layers() {
    // Placeholder for function to loop through array and return a vector of all the unique values
    // TODO: implment properly

    for (int i = 1; i <= 7; i++) {
      layers.push_back(i);
    }

  }


  void Deform_Volume::find_mesh_bounds() {
    // Find the first/last non 0 element along each dimensional
    // by looping over all elments

    // reset bounds
    xmax = INT_MIN, ymax = INT_MIN, zmax = INT_MIN;
    xmin = INT_MAX, ymin = INT_MAX, zmin = INT_MAX;

    int x,y,z;
    long idx;

    for (x = 0; x < dims; x++) {
      for (y = 0; y < dims; y++) {
        for (z = 0; z < dims; z++) {

          // TODO: dimensions of inr data are assumed to be in z,y,x order here
          // Not necessarily true in all cases?

          idx = get_array_index(z, y, x);
          // Is this element non 0 i.e. not background
          // if so check if the min/max values should be updated
          if (image_data[idx] != 0) {

            if (x > xmax) xmax = x;
            if (x < xmin) xmin = x;

            if (y > ymax) ymax = y;
            if (y < ymin) ymin = y;

            if (z > zmax) zmax = z;
            if (z < zmin) zmin = z;

          }
        }
      }
    }

    printf("Bounds of image are X: %d %d  Y: %d %d  Z: %d %d\n", xmin, xmax,
    ymin, ymax, zmin, zmax);

    xmid = (xmin + xmax)/2;
    ymid = (ymin + ymax)/2;
    zmid = (zmin + zmax)/2;
  }

  /*
  ########################################################
  End of Warper class
  ############################################################
  */
