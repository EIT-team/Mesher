#include "catch.hpp"
#include "mesh_operations.h"
#include <iostream>

using namespace CGAL::parameters;
using namespace std;


TEST_CASE ("Unit Cube") {

  // Unit cube (1 x 1 1)
  const char* inr_path = "./unit_cube.inr";

  CGAL::Image_3 image;
  image.read(inr_path);


  SECTION ("Read inr file and check dims") {
    // Check the input file is as expected
    REQUIRE (image.xdim() == 100);
    REQUIRE (image.ydim() == 100);
    REQUIRE (image.zdim() == 100);
    REQUIRE (image.vx() == 0.02);
    REQUIRE (image.vy() == 0.02);
    REQUIRE (image.vz() == 0.02);
  }

cout << "Creating mesh" << endl;

Mesh_domain domain(image);
Mesh_criteria criteria(facet_angle=30, facet_size=0.03, facet_distance=1, cell_radius_edge_ratio=3, cell_size=0.03);

C3t3 c3t3;
c3t3 = CGAL::make_mesh_3<C3t3>(domain, criteria, CGAL::parameters::features(domain),
CGAL::parameters::no_lloyd(), CGAL::parameters::no_odt(),
CGAL::parameters::no_perturb(),CGAL::parameters::no_exude());


SECTION ("Check mesh operation") {

  // Centre of mesh should be approx 1,1,1. Exact values depend on complexity of mesh
  Point centre = centre_of_mesh(c3t3);
  int expected_centre = 1;
  double margin = 0.03;
  //centre should be within 0.01 of expected value
  REQUIRE (expected_centre == Approx( CGAL::to_double( centre.x() )).margin(margin) );
  REQUIRE (expected_centre == Approx( CGAL::to_double( centre.y() )).margin(margin) );
  REQUIRE (expected_centre == Approx( CGAL::to_double( centre.z() )).margin(margin) );

  Point min_elem(0,0,0);
  double expected_min_close = 0.5;

  Point max_elem(100,100,100);
  double expected_max_close = 1.5;
  int target_domain = 1; // only one domain in cube
  Point closest_to_min = closest_element(c3t3, min_elem, target_domain);

  REQUIRE (expected_min_close == Approx( CGAL::to_double( closest_to_min.x() )).margin(margin) );
  REQUIRE (expected_min_close == Approx( CGAL::to_double( closest_to_min.y() )).margin(margin) );
  REQUIRE (expected_min_close == Approx( CGAL::to_double( closest_to_min.z() )).margin(margin) );

  Point closest_to_max = closest_element(c3t3, max_elem, target_domain);

  REQUIRE (expected_max_close == Approx( CGAL::to_double( closest_to_max.x() )).margin(margin) );
  REQUIRE (expected_max_close == Approx( CGAL::to_double( closest_to_max.y() )).margin(margin) );
  REQUIRE (expected_max_close == Approx( CGAL::to_double( closest_to_max.z() )).margin(margin) );

//TODO: test more mesh_operations
}

}
