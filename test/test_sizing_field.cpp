#include "catch.hpp"
#include "mesh_operations.h"
#include "Sizing_fields.h"
#include "input_parameters.h"

#include <iostream>

using namespace CGAL::parameters;
using namespace std;

#include "write_c3t3_to_vtk_xml_file.h"



TEST_CASE ("Sizing Fields on Unit Cube") {

    // Unit cube (dimensions 1 x 1 x 1) centred around 0.5,0.5,0.5

    const char* inr_path = "../test/unit_cube.inr";

    CGAL::Image_3 image;
    image.read(inr_path);

    // Read input file with parameters
    char *path_parameter = (char*)"./input_idx.txt";
    std::map<std::string, FT> options =  load_file_idx(path_parameter);


    options["vx"] = image.vx();
    options["vy"] = image.vy();
    options["vz"] = image.vz();

    options["xdim"] = image.xdim();
    options["ydim"] = image.ydim();
    options["zdim"] = image.zdim();
    char * path_electrode = "electrodes.txt"; // Empty file (TODO: add electrode test?)

    Point origin(image.vx () * image.xdim ()/2,
    image.vy () * image.ydim ()/2,
    image.vz () * image.zdim ()/2); //origin

    SECTION ("Spherical sizing field") {

      options["sphere_refinement"] = 1;

      Sizing_field sizing_field (origin,path_electrode,options);

      cout << "Creating mesh with sperical refinement" << endl;

      Mesh_domain domain(image);
      Mesh_criteria criteria( facet_angle=options["facet_angle_deg"],
                            facet_size=sizing_field,
                            facet_distance=options["facet_distance_mm"],
                            cell_radius_edge_ratio=options["cell_radius_edge_ratio"],
                             cell_size=sizing_field);

       C3t3_EIT c3t3;
       c3t3 = CGAL::make_mesh_3<C3t3_EIT>(domain, criteria, CGAL::parameters::features(domain),
       CGAL::parameters::no_lloyd(), CGAL::parameters::no_odt(),
       CGAL::parameters::no_perturb(),CGAL::parameters::no_exude());

       int vtk_success = write_c3t3_to_vtk_xml_file(c3t3, "cube_spherical_sizing_field.vtu");


    }
    SECTION ("Elliptic sizing field") {

      Sizing_field sizing_field (origin,path_electrode,options);

          cout << "Creating mesh with elliptic refinement" << endl;

      Mesh_domain domain(image);
      Mesh_criteria criteria( facet_angle=options["facet_angle_deg"],
                            facet_size=sizing_field,
                            facet_distance=options["facet_distance_mm"],
                            cell_radius_edge_ratio=options["cell_radius_edge_ratio"],
                             cell_size=sizing_field);

       C3t3_EIT c3t3;
       c3t3 = CGAL::make_mesh_3<C3t3_EIT>(domain, criteria, CGAL::parameters::features(domain),
       CGAL::parameters::no_lloyd(), CGAL::parameters::no_odt(),
       CGAL::parameters::no_perturb(),CGAL::parameters::no_exude());

       int vtk_success = write_c3t3_to_vtk_xml_file(c3t3, "cube_elliptic_sizing_field.vtu");


}

SECTION ("Planar sizing field") {

  options["planar_refinement"] = 1;
  options["upper_bound"] = image.vx() * image.xdim(); // x/y/z are equal for the unit cube., so can use either one here

  Sizing_field sizing_field (origin,path_electrode,options);

      cout << "Creating mesh with planar refinement" << endl;

  Mesh_domain domain(image);
  Mesh_criteria criteria( facet_angle=options["facet_angle_deg"],
                        facet_size=sizing_field,
                        facet_distance=options["facet_distance_mm"],
                        cell_radius_edge_ratio=options["cell_radius_edge_ratio"],
                         cell_size=sizing_field);

   C3t3_EIT c3t3;
   c3t3 = CGAL::make_mesh_3<C3t3_EIT>(domain, criteria, CGAL::parameters::features(domain),
   CGAL::parameters::no_lloyd(), CGAL::parameters::no_odt(),
   CGAL::parameters::no_perturb(),CGAL::parameters::no_exude());

   int vtk_success = write_c3t3_to_vtk_xml_file(c3t3, "cube_planar_sizing_field.vtu");


}
}
