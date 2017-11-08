#ifndef MESH_OPS_H
#define MESH_OPS_H

#include "CGAL_include.h"
#include "include.h"

using namespace std;

vector<Point> load_electrode_locations(FILE *, FT );

class C3t3_EIT : public C3t3 {

  // Repeating this here from CGAL_include.h as won't compile without it
  // as it doesn't recognise Point as a return type for  functions
  // If the return type if a standard type (int/double etc) there are no problems
  // Also compiles fine if class is not derived from C3t3
  // TODO: try and fix this

  typedef CGAL::Exact_predicates_inexact_constructions_kernel::Point_3 Point;

    public:

      double x_min = DBL_MAX, y_min = DBL_MAX, z_min = DBL_MAX;
      double x_max = -DBL_MAX, y_max = -DBL_MAX, z_max = -DBL_MAX;

      void find_mesh_bounds();
      int get_outer_layer_domain();
      Point centre_of_mesh();
      Point closest_element(Point, int);
      Point set_reference_electrode();
      Point set_ground_electrode();

};

#endif
