#ifndef MESH_OPS_H
#define MESH_OPS_H

#include "CGAL_include.h"
#include "include.h"
#include <cmath>

using namespace std;

vector<Point> load_electrode_locations(FILE *, FT);

class C3t3_EIT : public C3t3
{
  // Repeating this here from CGAL_include.h as won't compile without it
  // as it doesn't recognise Point as a return type for  functions
  // If the return type if a standard type (int/double etc) there are no problems
  // Also compiles fine if class is not derived from C3t3

  typedef CGAL::Exact_predicates_inexact_constructions_kernel::Point_3 Point;

public:
  double x_min = DBL_MAX, y_min = DBL_MAX, z_min = DBL_MAX;
  double x_max = -DBL_MAX, y_max = -DBL_MAX, z_max = -DBL_MAX;
  double x_mid = 0, y_mid = 0, z_mid = 0;

  void find_mesh_bounds();
  int get_outer_layer_domain();

  Point find_centre_of_mesh();
  Point find_closest_element(Point, int);
  Point set_reference_electrode_human();
  Point set_ground_electrode_human();
};

std::vector<Point> tetra_cell_to_points(Cell_handle);
double tetra_volume(std::vector<Point>);
double tetra_squared_edge_length(std::vector<Point>);
double tetra_quality(std::vector<Point>);
std::vector<double> check_mesh_quality(C3t3_EIT &);
void check_CGAL_opt_code(CGAL::Mesh_optimization_return_code);
void optimise_mesh(C3t3_EIT &, Mesh_domain &, map<string, FT>);

#endif
