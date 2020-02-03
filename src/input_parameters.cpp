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

  validate_mandatory_params(options);

  return options;
}

/* Check that each string is 'expected_parms' is a key in the options map.
Throw an error if not found */
void validate_params(map<string, FT> options, vector<string> expected_params) {
  
  for (auto param : expected_params)
  {
    if (!options.count(param)) {
      cout << param << " parameter not found!" << endl;
      throw runtime_error("Exiting");
    }
  }
}

void validate_mandatory_params(std::map<std::string, FT> options) {
      vector<string> expected_params = {"planar_refinement",
                                        "depth_refinement",
                                        "electrode_refinement",
                                        "sphere_refinement",
                                        "cuboid_refinement",
                                        "facet_angle_deg",
                                        "facet_distance_mm",
                                        "cell_radius_edge_ratio",
                                        "cell_fine_size_mm",
                                        "cell_coarse_size_mm",
                                        "lloyd_opt",
                                        "odt_opt",
                                        "exude_opt",
                                        "perturb_opt",
                                        "time_limit_sec",
                                        "save_vtk",
                                        "save_cell_centres",
                                        "save_nodes_tetra",
                                        "do_deformation"};
  
  validate_params(options, expected_params);
}