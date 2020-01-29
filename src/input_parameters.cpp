#include "input_parameters.h"

using namespace std;

/** Read mesher parameters from file.
 * Takes file name as parameter.
 * Returns map of parameters.
 **/
map<string, FT> read_params_from_file(char *file_name_input)
{
  map<string, FT> options;

  ifstream cfgfile(file_name_input, ifstream::in);
  if (!cfgfile)
    perror("\n Error opening input parameters file");

  string line;

  cout << "Reading parameters from: " << file_name_input << endl;

  string id, eq;
  FT val;

  while (getline(cfgfile, line))

  {
    stringstream ss(line);

    if (ss.peek() == '#')
      continue; // skip comments

    ss >> id >> eq >> val;

    if (eq != "=")
    {
      throw runtime_error("Input parameters file parse error: has to be variable[space]=[space]value");
    }

    options[id] = val;
  }

  //The voxel size in the input mesh can vary,
  //Scale everything so that it is in mm

  FT unit = options["pixel_scale_mm"]; //Scaling units - voxels per mm

  options["sphere_radius"];
  options["sphere_centre_x"];
  options["sphere_centre_y"];
  options["sphere_centre_z"];
  options["sphere_cell_size"];
  options["cuboid_x_extent"];
  options["cuboid_y_extent"];
  options["cuboid_z_extent"];
  options["cuboid_centre_x"];
  options["cuboid_centre_y"];
  options["cuboid_centre_z"];
  options["cuboid_cell_size"];

  return options;
}
