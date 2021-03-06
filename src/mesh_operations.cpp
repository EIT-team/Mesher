#include "mesh_operations.h"
#include "CGAL_include.h"

using namespace CGAL::parameters;
using namespace std;

/**
 * Returns centre point of the mesh.
 **/
Point C3t3_EIT::find_centre_of_mesh()
{

  const Tr &tr = triangulation();
  double n_vertex = tr.number_of_vertices();

  double mean_vtx[3] = {0};
  Point p;
  map<Vertex_handle, int> V;
  int inum = 1;

  for (Finite_vertices_iterator vit = tr.finite_vertices_begin(); vit != tr.finite_vertices_end(); ++vit)
  {
    p = Point(vit->point());
    mean_vtx[0] += CGAL::to_double(p.x());
    mean_vtx[1] += CGAL::to_double(p.y());
    mean_vtx[2] += CGAL::to_double(p.z());
  }

  Point centre_point(mean_vtx[0] / n_vertex, mean_vtx[1] / n_vertex, mean_vtx[2] / n_vertex);

  cout << "x: " << centre_point.x() << endl
       << "y: " << centre_point.y() << endl
       << "z: " << centre_point.z() << endl;

  return centre_point;
}

/**
 * Find the closest element in the surface to a given point in space,
 * The found point will be in the specified domain (tissue type)
 * */
Point C3t3_EIT::find_closest_element(Point target_p, int target_domain)
{

  double min_dist = DBL_MAX;
  double this_dist;
  Point centre_of_closest(0, 0, 0);
  vector<Point> facet_points;

  // Iterate over all facets
  for (Facet_iterator facet_iterator = facets_in_complex_begin();
       facet_iterator != facets_in_complex_end(); ++facet_iterator)
  {

    // Get domain number/tissue type using facet map
    int this_tissue = facet_iterator->first->subdomain_index();

    // Check facet is the tissue type we want
    if (this_tissue == target_domain)
    {
      for (int i = 0; i < 4; ++i)
      {

        // We only want to store the three verticies of the cell that correspond to this facet
        // ->second gives the index of the vertex opposite to the facet, which is the one we don't want
        if (i != facet_iterator->second)
        {
          facet_points.push_back(Point(facet_iterator->first->vertex(i)->point()));
        }
      }

      //Calculate centre of the facet
      Point centre_of_facet = CGAL::centroid(facet_points[0], facet_points[1], facet_points[2]);
      //cout << "Centre: " << centre_of_facet <<endl;

      // how far is this facet from the target point
      this_dist = CGAL::squared_distance(target_p, centre_of_facet);

      // If closer than current minimum, store it and update min distance
      if (this_dist < min_dist)
      {
        min_dist = this_dist;
        centre_of_closest = centre_of_facet;
      }

      // Empty facet_points, otherwise the [0] [1] [2] indexing used for CGAL::centroid won't do anything with new data
      facet_points.clear();
    }
  }

  cout << "Closest point to: " << target_p << " is: " << centre_of_closest << endl;
  return centre_of_closest;
}

/**
 * Return the domain of the outermost layer in the mesh by looking for the
 * element with the largest x coordinate.
 **/
int C3t3_EIT::get_outer_layer_domain()
{

  double max_x = -DBL_MAX;
  int outer_domain;
  Point this_point;

  // Iterate over all facets
  for (Facet_iterator facet_iterator = facets_in_complex_begin();
       facet_iterator != facets_in_complex_end(); ++facet_iterator)
  {

    // Get domain number/tissue type
    int this_tissue = facet_iterator->first->subdomain_index();

    for (int i = 0; i < 4; ++i)
    {

      if (i != facet_iterator->second)
      {
        this_point = Point(facet_iterator->first->vertex(i)->point());
      }
    }

    // Found a nex max_point, store the domain
    // As long as the domain isn't 0, as this is 'outside' of the mesh
    if (this_point[0] > max_x && this_tissue != 0)
    {
      max_x = this_point[0];
      outer_domain = facet_iterator->first->subdomain_index();
    }
  }

  return outer_domain;
}

/**
 * Place the ground location at the back of the head (For humans).
 * 
 * Returns location of electrode.
 **/
Point C3t3_EIT::set_ground_node()
{

  const Tr &tr = triangulation();
  double n_vertex = tr.number_of_vertices();
  Point gnd_electrode, current_vertex;
  double furthest = -1000; // Set to sentinel values
  double current_y;

  int inum = 1;

  // Find the largest y value by iterating through all vertices
  for (Finite_vertices_iterator vit = tr.finite_vertices_begin(); vit != tr.finite_vertices_end(); ++vit)
  {

    current_vertex = Point(vit->point());
    current_y = CGAL::to_double(current_vertex.y());

    // Check if current value is the largest and update if so
    if (current_y > furthest)
    {
      furthest = current_y;
      gnd_electrode = Point(current_vertex);
    }
  }

  cout << "Ground electrode placed at: " << gnd_electrode << endl;
  return gnd_electrode;
}

/**
 * Iterate through all verticies and find the min/max values in each dimensions.
 * These are the start/end bounds of the mesh.
 **/
void C3t3_EIT::find_mesh_bounds()
{

  const Tr &tr = triangulation();
  double n_vertex = tr.number_of_vertices();
  double x, y, z;

  // Iterate through all vertices
  for (Finite_vertices_iterator vit = tr.finite_vertices_begin(); vit != tr.finite_vertices_end(); ++vit)
  {

    auto current_vertex = vit->point();
    x = CGAL::to_double(current_vertex.x());
    y = CGAL::to_double(current_vertex.y());
    z = CGAL::to_double(current_vertex.z());

    // Check if current value is the bigger/smaller than current and update if so
    if (x > x_max)
      x_max = x;
    if (x < x_min)
      x_min = x;

    if (y > y_max)
      y_max = y;
    if (y < y_min)
      y_min = y;

    if (z > z_max)
      z_max = z;
    if (z < z_min)
      z_min = z;
  }

  x_mid = (x_min + x_max) / 2;
  y_mid = (y_min + y_max) / 2;
  z_mid = (z_min + z_max) / 2;
}

/**
 * Loads electrode positions from a file.
 * Returns a vector of points.
 **/
vector<Point> load_electrode_locations(FILE *F, FT scale)
{

  vector<Point> electrode_locations;

  if (F == NULL)
    perror("Error opening electrode file");
  else
  {
    while (!feof(F))
    {
      float x, y, z;
      int count = fscanf(F, "%f,%f,%f\n", &x, &y, &z);
      Point pt(x * scale, y * scale, z * scale);
      electrode_locations.push_back(pt);
    }
  }

  return electrode_locations;
}

/**
 * Return volume of a tetrahedral element.
 **/
double tetra_volume(vector<Point> vertices)
{

  if (vertices.size() != 4)
  {
    cout << "Invalid tetra passed - doesn't have 4 vertices!" << endl;
    return 0;
  }

  double volume = CGAL::volume(vertices[0], vertices[1], vertices[2], vertices[3]);
  return CGAL::abs(volume);
}

/** Convert a cell handle to  a vector of vertices, which can be passed to
 * another fuction.
 **/
vector<Point> tetra_cell_to_points(Cell_handle cell)
{
  vector<Point> vertices;

  // Put vertices in vector
  for (int i = 0; i < 4; i++)
  {
    vertices.push_back(Point(cell->vertex(i)->point()));
  }

  return vertices;
}

/**
 * Return sum of  squared edge lengths of tetra
 **/
double tetra_squared_edge_length(vector<Point> vertices)
{

  if (vertices.size() != 4)
  {
    cout << "Invalid tetra passed - doesn't have 4 vertices!" << endl;
    return 0;
  }

  int i, j;
  int n_vertex = 4;

  double total_length = 0;

  // Loop over all vertex pairs
  for (i = 0; i < n_vertex; i++)
  {
    for (j = 0; j < i; j++)
    {

      total_length += CGAL::squared_distance(vertices[i], vertices[j]);
    }
  }

  return total_length;
}
/**
   * Return the Joe-Liu quality of a tetrahedral element
   **/
double tetra_quality(vector<Point> vertices)
{

  double volume, edge_length, quality;

  volume = tetra_volume(vertices);
  edge_length = tetra_squared_edge_length(vertices);

  quality = (12 * pow((3 * volume), (2.0 / 3.0))) / edge_length;

  return quality;
}

/** Checks the quality of a mesh by calculatng the quality of each element.
 * 
 * Returns vector containing the number of low//high quality elements, and the average quality value.
 **/
vector<double> check_mesh_quality(C3t3_EIT &c3t3)
{

  cout << "Checking mesh quality" << endl;
  Cell_iterator cell_iterator;
  long num_cells = c3t3.number_of_cells_in_complex();
  double quality;

  double low_threshold = 0.1, high_threshold = 0.9;
  double low_quality = 0, high_quality = 0;
  double sum_quality = 0, average_quality = 0;

  vector<Point> vertices;
  vector<double> metrics; // Store some quality metrics about the mesh
  // Iterate over all cells and calculate the Joe-Liu quality of the element
  for (cell_iterator = c3t3.cells_in_complex_begin();
       cell_iterator != c3t3.cells_in_complex_end();
       ++cell_iterator)
  {

    // Convert cell handle to vertices vector
    vertices = tetra_cell_to_points(cell_iterator);

    // Calculate Joe-Liu quality
    quality = tetra_quality(vertices);
    sum_quality += quality;

    if (quality < low_threshold)
    {
      low_quality += 1;
      //cout << quality << endl;

      if (quality > high_threshold)
      {
        high_quality += 1;
      }
    }
  }

  average_quality = sum_quality /= num_cells;

  // Store the number of low/high quality cells, and the average quality
  metrics.push_back(low_quality);
  metrics.push_back(high_quality);
  metrics.push_back(average_quality);

  cout << "Low quality elements: " << low_quality << endl;
  cout << "High quality elements: " << high_quality << endl;
  cout << "Average quality of elements: " << average_quality << endl;

  return metrics;
}

void check_CGAL_opt_code(CGAL::Mesh_optimization_return_code opt_code)
{
  if (opt_code == CGAL::TIME_LIMIT_REACHED)
  {
    cout << "time limit reached";
  }
  else if ((opt_code == CGAL::CANT_IMPROVE_ANYMORE) || (opt_code == CGAL::CONVERGENCE_REACHED))
  {
    cout << "done, cannot improve anymore";
  }
  else
  {
    cout << "done";
  }

  cout << endl;
}

/* Run mesh optimsation */
void optimise_mesh(C3t3_EIT &c3t3, Mesh_domain &domain, map<string, FT> options)
{
  CGAL::Mesh_optimization_return_code opt_code;
  cout << "Optimising Mesh" << endl;

  vector<double> mesh_quality_metrics(3);
  mesh_quality_metrics = check_mesh_quality(c3t3);
  cout << "Number of tetra before optimisation: " << c3t3.number_of_cells_in_complex() << endl;

  if (int(options["odt_opt"]) == 1)
  {
    cout << "ODT... " << flush;
    opt_code = CGAL::odt_optimize_mesh_3(c3t3, domain, time_limit = options["time_limit_sec"]);
    check_CGAL_opt_code(opt_code);
  }

  if (int(options["lloyd_opt"]) == 1)
  {
    cout << "Lloyd... " << flush;
    opt_code = CGAL::lloyd_optimize_mesh_3(c3t3, domain, time_limit = options["time_limit_sec"]);
    check_CGAL_opt_code(opt_code);
  }

  if (int(options["perturb_opt"]) == 1)
  {
    cout << "Perturb... " << flush;
    opt_code = CGAL::perturb_mesh_3(c3t3, domain, sliver_bound = 10, time_limit = options["time_limit_sec"]);
    check_CGAL_opt_code(opt_code);
  }

  if (int(options["exude_opt"]) == 1)
  {
    cout << "Exude... " << flush;
    opt_code = CGAL::exude_mesh_3(c3t3, sliver_bound = 10, time_limit = options["time_limit_sec"]);
    check_CGAL_opt_code(opt_code);
  }

  // check mesh quality again to show improvement
  mesh_quality_metrics = check_mesh_quality(c3t3);
  cout << "Number of tetra after optimisation: " << c3t3.number_of_cells_in_complex() << endl;
}
