#include "catch.hpp"

#include "deform_volume.h"
#include "mesh_operations.h"
#include "input_parameters.h"

#include "write_c3t3_to_vtk_xml_file.h"

using namespace CGAL::parameters;
using namespace std;

using namespace std;

TEST_CASE("Array indexing")
{
  Deform_Volume warper;
  int dims = 5;
  warper.xdim = dims;
  warper.ydim = dims;
  warper.zdim = dims;

  //Setup
  int i, j, k;
  long idx;

  SECTION("Indexing 'normal' values")
  {

    for (i = 0; i < dims; i++)
    {
      for (j = 0; j < dims; j++)
      {
        for (k = 0; k < dims; k++)
        {

          idx = warper.get_array_index(i, j, k);
          REQUIRE(idx == (i * dims + j) * dims + k);
        }
      }
    }

    SECTION("Indexing large dimension array")
    {
      // Test for final elment of typical array size
      int big_dim = 512;
      warper.xdim = big_dim;
      warper.ydim = big_dim;
      warper.zdim = big_dim;

      int max_array_idx = big_dim - 1; // due to 0 indexing

      idx = warper.get_array_index(max_array_idx, max_array_idx, max_array_idx);

      REQUIRE(idx == big_dim * big_dim * big_dim - 1);
    }
  }

  SECTION("Trying to index values that don't exist")
  {

    int too_big = dims + 1;
    REQUIRE(warper.get_array_index(too_big, too_big, too_big) == -1);
    REQUIRE(warper.get_array_index(-1, -1, -1) == -1);
  }
}

TEST_CASE("Neighbouring elements to first elment")
{

  Deform_Volume warper;
  int dims = 5;
  warper.xdim = dims;
  warper.ydim = dims;
  warper.zdim = dims;
  long idx;
  int expected_neighbours = 7;

  // Check element 0
  idx = 0;

  vector<long> neighbours = warper.neighbouring_elements(idx);

  long expected[] = {1, 5, 6, 25, 26, 30, 31};
  vector<long> expected_results(expected, expected + sizeof(expected) / sizeof(long));

  // Sort vector for consistent results
  sort(neighbours.begin(), neighbours.end());

  REQUIRE(neighbours.size() == expected_neighbours);
  REQUIRE(neighbours == expected_results);
}

TEST_CASE("Neighbouring elements to last corner")
{

  Deform_Volume warper;
  int dims = 5;
  warper.xdim = dims;
  warper.ydim = dims;
  warper.zdim = dims;
  long idx;

  int expected_neighbours = 7;
  idx = 124;

  vector<long> neighbours = warper.neighbouring_elements(idx);

  long expected[] = {93, 94, 98, 99, 118, 119, 123};
  vector<long> expected_results(expected, expected + sizeof(expected) / sizeof(long));

  // Sort vector for consistent results
  sort(neighbours.begin(), neighbours.end());

  REQUIRE(neighbours.size() == expected_neighbours);
  REQUIRE(neighbours == expected_results);
}

TEST_CASE("Neighbouring elements to centre")
{

  Deform_Volume warper;
  int dims = 5;
  warper.xdim = dims;
  warper.ydim = dims;
  warper.zdim = dims;
  long idx;

  int expected_neighbours = 26;
  idx = 62;

  vector<long> neighbours = warper.neighbouring_elements(idx);

  // Sort vector for consistent results
  sort(neighbours.begin(), neighbours.end());

  long expected[] = {31, 32, 33, 36, 37, 38, 41, 42, 43,
                     56, 57, 58, 61, 63, 66, 67, 68,
                     81, 82, 83, 86, 87, 88, 91, 92, 93};

  vector<long> expected_results(expected, expected + sizeof(expected) / sizeof(long));

  REQUIRE(neighbours.size() == expected_neighbours);
  REQUIRE(neighbours == expected_results);
}

TEST_CASE("Dilation")
{

  Deform_Volume warper;
  int dims = 5;
  warper.xdim = dims;
  warper.ydim = dims;
  warper.zdim = dims;
  int total_elements = dims * dims * dims;

  unsigned char test_input[dims * dims * dims];
  warper.image_data = test_input;

  int i, j, k;
  int central_element = 62; // centre of 5x5x5
  long idx;

  unsigned char layer_0 = (unsigned char)0;
  unsigned char layer_1 = (unsigned char)1;

  for (i = 0; i < total_elements; i++)
  {
    test_input[i] = layer_0;
  }

  SECTION("Dilate central element")
  {

    // Set central element to 1
    test_input[central_element] = layer_1;

    warper.dilate_layer(1, 1);

    // Check some values that should not have changed
    REQUIRE(test_input[0] == layer_0);
    REQUIRE(test_input[2] == layer_0);
    REQUIRE(test_input[dims * dims * dims - 1] == layer_0);

    // All these values should have changed
    long expected[] = {31, 32, 33, 36, 37, 38, 41, 42, 43,
                       56, 57, 58, 61, 63, 66, 67, 68,
                       81, 82, 83, 86, 87, 88, 91, 92, 93};
    vector<long> expected_results(expected, expected + sizeof(expected) / sizeof(long));

    long current_idx;

    while (!expected_results.empty())
    {

      current_idx = expected_results.back();
      REQUIRE(test_input[current_idx] == layer_1);
      expected_results.pop_back();
    }
  }

  SECTION("Dilate by 2 pixels")
  {
    test_input[central_element] = layer_1;

    // This should set all elements to 1
    warper.dilate_layer(1, 2);

    // Check all elements are 1
    for (i = 0; i < total_elements; i++)
    {
      REQUIRE(test_input[i] == layer_1);
    }
  }
}

TEST_CASE("Identify unique layers in unit cube")
{
  /* Check that get_layers() function is working correctly */

  const char *inr_path = "../test/unit_cube.inr";
  CGAL::Image_3 image;
  image.read(inr_path);

  char *path_parameter = (char *)"./input_idx.txt";
  std::map<std::string, FT> options = read_params_from_file(path_parameter);

  Deform_Volume warper(&image, options);

  warper.get_layers();
  std::vector<int> expected = {1};
  REQUIRE(warper.layers == expected);

  // Change one of the elments to a new layer
  int new_layer = 2;
  warper.image_data[5] = new_layer;
  warper.get_layers();
  expected.push_back(new_layer);

  REQUIRE(warper.layers == expected);

  new_layer = 5;
  warper.image_data[7] = new_layer;
  warper.get_layers();
  expected.push_back(new_layer);

  REQUIRE(warper.layers == expected);
}

TEST_CASE("Random deformation on brain mesh")
{
  /* Do some random deformations on an .inr file, which has
  different x/y/z dimensions. */

  const char *inr_path = "../examples/unitcube/unit_cube.inr";

  char *path_parameter = (char *)"./input_idx.txt";
  std::map<std::string, FT> options = read_params_from_file(path_parameter);

  CGAL::Image_3 image;
  image.read(inr_path);

  Deform_Volume warper(&image, options);

  warper.modify_image();

  cout << "Creating mesh" << endl;

  Mesh_domain domain(image);
  Mesh_criteria criteria(facet_angle = 30, facet_size = 4, facet_distance = 5, cell_radius_edge_ratio = 3, cell_size = 8);

  C3t3_EIT c3t3;
  c3t3 = CGAL::make_mesh_3<C3t3_EIT>(domain, criteria, CGAL::parameters::features(domain),
                                     CGAL::parameters::no_lloyd(), CGAL::parameters::no_odt(),
                                     CGAL::parameters::no_perturb(), CGAL::parameters::no_exude());
}
