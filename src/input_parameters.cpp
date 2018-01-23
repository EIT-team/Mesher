#include "input_parameters.h"

using namespace std;

std::map<std::string, FT> load_file_idx(char* file_name_input)
{
  std::map<std::string, FT> options;

  std::ifstream cfgfile(file_name_input, std::ifstream::in); // = std::ifstream::open(file_name_input);
  if (!cfgfile) perror ("\n Error opening input parameters file");

  string line;

  cout << "Reading parameters from: " << file_name_input << endl;

  std::string id, eq;
  FT val;

  //while(cfgfile >> id >> eq >> val)
  while (getline( cfgfile, line) )

  {
    stringstream ss (line);

    if ( ss.peek() == '#') continue; // skip comments

    ss >> id >> eq >> val;

    if (eq != "=") throw std::runtime_error("Input parameters file parse error: has to be variable[space]=[space]value");

    options[id] = val;
    std::cout << id <<" = "<< options[id] << "\n";
  }

  //The voxel size in the input mesh can vary,
  //Scale everything so that it is in mm
  
  FT unit = options["pixel_scale_mm"]; //Scaling units - mm per pixel
  options["facet_distance_mm"]/=unit;
  options["cell_fine_size_mm"]/=unit;
  options["cell_coarse_size_mm"]/=unit;
  options["cell_size_electrodes_mm"]/=unit;
  options["electrode_radius_mm"]/=unit;
  options["sphere_radius"]/=unit;
  options["sphere_centre_x"]/=unit;
  options["sphere_centre_y"]/=unit;
  options["sphere_centre_z"]/=unit;
  options["sphere_cell_size"]/=unit;




  return options;


}

vector< vector<double> > load_deformations(const char* file_name_input) {

  /* Read in a series of deformation parmaters from a file
  Each line of the file should contain 3 doubles (x/y/z deformation in mm)
  */

  vector< vector<double> > deformations;

  ifstream deform_file("list_of_deformations.txt", std::ifstream::in);
  if (!deform_file) {
    perror ("\n Error opening deformations file");
  }

  cout << "Reading deformations from fie." << endl;


  double dist_x, dist_y, dist_z;

  while(deform_file >> dist_x >> dist_y >> dist_z)
  {
    cout << dist_x << " " << dist_y << " " << dist_z <<endl;
    //TODO: Check sensible values

    vector<double> this_deform;
    this_deform.push_back(dist_x);
    this_deform.push_back(dist_y);
    this_deform.push_back(dist_z);

    deformations.push_back(this_deform);

  }

  return deformations;

}
