#include "input_parameters.h"

using namespace std;

map<string, FT> read_params_from_file(char *file_name_input)
{
  map<string, FT> options;

  ifstream cfgfile(file_name_input, ifstream::in);
  if (!cfgfile)
    perror("\n Error opening input parameters file");

  string line;

  cout << "Reading parameters from: " << file_name_input << endl;

  sstring id, eq;
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

  //TODO: Set pixel scale automatically by reading the value from the inr file
  FT unit = options["pixel_scale_mm"]; //Scaling units - voxels per mm

  options["sphere_radius"] /= unit;
  options["sphere_centre_x"] /= unit;
  options["sphere_centre_y"] /= unit;
  options["sphere_centre_z"] /= unit;
  options["sphere_cell_size"] /= unit;
  options["square_x_extent"] /= unit;
  options["square_y_extent"] /= unit;
  options["square_z_extent"] /= unit;
  options["square_centre_x"] /= unit;
  options["square_centre_y"] /= unit;
  options["square_centre_z"] /= unit;
  options["square_cell_size"] /= unit;

  return options;
}

vector<vector<double>> load_deformations(const char *file_name_input)
{

  /* Read in a series of deformation paramters from a file
  Each line of the file should contain 3 doubles (x/y/z deformation in mm)
  */

  vector<vector<double>> deformations;

  ifstream deform_file("list_of_deformations.txt", ifstream::in);
  if (!deform_file)
  {
    perror("\n Error opening deformations file");
  }

  cout << "Reading deformations from fie." << endl;

  double dist_x, dist_y, dist_z;

  while (deform_file >> dist_x >> dist_y >> dist_z)
  {
    cout << dist_x << " " << dist_y << " " << dist_z << endl;
    //TODO: Check sensible values

    vector<double> this_deform;
    this_deform.push_back(dist_x);
    this_deform.push_back(dist_y);
    this_deform.push_back(dist_z);

    deformations.push_back(this_deform);
  }

  return deformations;
}
