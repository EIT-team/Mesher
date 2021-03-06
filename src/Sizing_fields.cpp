//Contains different sizing fields structures for different types of refinement.
// See CGAL documentation (https://doc.cgal.org/latest/Mesh_3/index.html#title27) for more details on sizing fields.

#include "Sizing_fields.h"
using namespace std;

Sizing_field::Sizing_field(Point &origin_in, string path_electrode, std::map<std::string, FT> opts)
{

  FILE *F;
  try
  {
    F = fopen(path_electrode.c_str(), "r");
  }
  catch (exception &e)
  {
    cout << e.what() << endl;
  }

  options = opts;
  origin = origin_in;

  ub_x = origin.x();
  ub_y = origin.y();
  ub_z = origin.z();

  // Reallocate some parameters to have less verbose names
  coarse_size = options["cell_coarse_size_mm"];
  fine_size = options["cell_fine_size_mm"];

  if (options["planar_refinement"])
  {
    validate_planar_params(options);
    if (options["planar_direction_xyz"] == 1)
    {
      options["height"] *= options["vx"];
      options["upper_bound"] = options["vx"] * options["xdim"];
    }

    else if (options["planar_direction_xyz"] == 2)
    {
      options["height"] *= options["vy"];
      options["upper_bound"] = options["vy"] * options["ydim"];
    }

    else if (options["planar_direction_xyz"] == 3)
    {
      options["height"] *= options["vz"];
      options["upper_bound"] = options["vz"] * options["zdim"];
    }

    else
    { // Invalid parameter passed
      cout << "Invalid planar direction specified, should be 1, 2 or 3" << endl;
      exit(0);
    }
  }

  // Load electrode positions
  if (F == NULL)
    perror("Error opening electrode file");
  else
  {
    while (!feof(F))
    {
      float x, y, z;
      int count = fscanf(F, "%f,%f,%f\n", &x, &y, &z);
      Point pt(x * options["vx"], y * options["vy"], z * options["vz"]);
      centres.push_back(pt);
    }
  }

  if (F != NULL)
    fclose(F);
}

/** Overload the Sizing field operator to implement custom refinement.
 * Function parameters are specified in the CGAL docs (see link at top for example)
 * 
 * Each point in the mesh will be passed to this function, returning the cell size at that point.
 * 
 * Points near to an electrode are always refined.
 * Spherical, cuboid, planar can be set in the parameter file. If none are set, elliptical refinment is
 * used by default, where the cells closer to the centre are refined more than those further away 
 **/
FT Sizing_field::operator()(const Point &p, const int, const Index &) const

{

  double distance, distance_x, distance_y, distance_z;

  FT out = coarse_size; //default value, can be changed by the below sizing fields

  // Do some  refinements if turned on in parameter file
  // Need to use MAP.at("x") rather than MAP["x"] to be const safe

  if (options.at("planar_refinement"))
  {
    validate_planar_params(options);

    if (options.at("planar_direction_xyz") == 1)
    {
      distance = CGAL::abs(p.x() - options.at("height"));
    }

    else if (options.at("planar_direction_xyz") == 2)
    {
      distance = CGAL::abs(p.y() - options.at("height"));
    }

    else if (options.at("planar_direction_xyz") == 3)
    {
      distance = CGAL::abs(p.z() - options.at("height"));
    }

    else
    {
      //return fine_size; //If specified wrong it should leave out untouched
    }

    double dist_percentage = distance / double(options.at("upper_bound"));
    int fine_size_percentage = int(options.at("elements_with_fine_sizing_field_percentage"));

    if (dist_percentage <= (fine_size_percentage) / 100.0)
    {
      out = fine_size;
    }

    else
    {
      double out_tmp = fine_size +
            CGAL::abs((coarse_size - fine_size) * (dist_percentage - (fine_size_percentage) / 100.0));

      // clamp sizes within range specified       
      if (out_tmp > coarse_size)
      {
        out_tmp = coarse_size;
      }
      out = out_tmp;

    }
  }

  { //refine centre of mesh more than outside
  if (options.at("depth_refinement"))

    validate_depth_params(options);

    // Calculate cartersian distance from centre of the mesh
    Vector distance_elliptic = p - origin;
    FT distance_percent = CGAL::sqrt((distance_elliptic.x() / origin.x()) * (distance_elliptic.x() / origin.x()) +
                                     (distance_elliptic.y() / origin.y()) * (distance_elliptic.y() / origin.y()) +
                                     (distance_elliptic.z() / origin.z()) * (distance_elliptic.z() / origin.z()));

    int fine_size_percentage = int(options.at("elements_with_fine_sizing_field_percentage"));

    if (distance_percent >= 1 - FT(fine_size_percentage) / 100)
    {
      double out_tmp = fine_size + (coarse_size - fine_size) * (1 - distance_percent);

      //clamp sizes within range specified
      if (out_tmp < fine_size)
      {
        out_tmp=fine_size;
      }
      out = out_tmp;
    }

    else
    {
      out = coarse_size;
    }
  }

  if (options.at("sphere_refinement"))
  {
    validate_sphere_params(options);
    Point sphere_centre(options.at("sphere_centre_x"),
                        options.at("sphere_centre_y"),
                        options.at("sphere_centre_z"));

    distance = CGAL::sqrt(CGAL::squared_distance(p, sphere_centre));

    if (distance < FT(options.at("sphere_radius")))
    {

      out = options.at("sphere_cell_size");
    }
  }

  if (options.at("cuboid_refinement")) {
   validate_cuboid_params(options);
  
    // Refine a cuboid around a specificed point.

    Point cuboid_centre(options.at("cuboid_centre_x"),
                        options.at("cuboid_centre_y"),
                        options.at("cuboid_centre_z"));

    distance_x = CGAL::abs(p.x() - cuboid_centre.x());
    distance_y = CGAL::abs(p.y() - cuboid_centre.y());
    distance_z = CGAL::abs(p.z() - cuboid_centre.z());

    if (distance_x < FT(options.at("cuboid_x_extent")) && distance_y < FT(options.at("cuboid_y_extent")) && distance_z < FT(options.at("cuboid_z_extent")))
    {

      out = options.at("cuboid_cell_size");
    }
  }

  if (options.at("electrode_refinement"))
  {
    validate_electrode_params(options);
    FT e_R = 2 * options.at("electrode_radius_mm"); // 2 * to secure fit of the electrode

    Points::const_iterator it;
    for (it = centres.begin(); it < centres.end(); it++)
    {
      Vector pp = (p - *it);
      if (pp.squared_length() <= e_R * e_R)
      {

        out = options.at("cell_size_electrodes_mm");;
        return out;
      }
    }
  }

  return out;
}

void validate_sphere_params(map<string, FT> options) {
    vector<string> expected_params = {"sphere_radius",
                                     "sphere_centre_x",
                                     "sphere_centre_y",
                                     "sphere_centre_z",
                                     "sphere_cell_size"};

    validate_params(options, expected_params);
}

//TODO List cube values. Check that examples/refinment has cuboid_ rather than square_

void validate_cuboid_params(map<string, FT> options) {
    vector<string> expected_params = {"cuboid_x_extent",
                                     "cuboid_y_extent",
                                     "cuboid_z_extent",
                                     "cuboid_centre_x",
                                     "cuboid_centre_z",
                                     "cuboid_centre_y",
                                     "cuboid_cell_size"};
    validate_params(options, expected_params);
}

void validate_planar_params(map<string, FT> options) {
    vector<string> expected_params = {"height",
                                     "planar_direction_xyz",
                                     "elements_with_fine_sizing_field_percentage"};
    validate_params(options, expected_params);
}

void validate_depth_params(map<string, FT> options) {
  vector<string> expected_params = {"elements_with_fine_sizing_field_percentage"};
  validate_params(options, expected_params);
}

void validate_electrode_params(map<string, FT> options) {
    vector<string> expected_params = {"cell_size_electrodes_mm",
                                      "electrode_radius_mm"};
                                      
    validate_params(options, expected_params);
}
