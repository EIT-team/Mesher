
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

//#include "warp_mesh.h"
#include "stretch_info.h"

//#include <algorithm> // std::sort

using namespace std;

TEST_CASE("Stretching") {

  int dims = 512;
  int anchor = 256;
  int distance = 25;
  int left_point = 50;
  int right_point = 300;

  Stretch_Info stretch_left(left_point, distance, anchor, dims);
  Stretch_Info stretch_right(right_point, distance, anchor, dims);

  SECTION("Check valid stretch parmeters") {


    REQUIRE( stretch_left.step == 1);
    REQUIRE( stretch_left.active == true );
    REQUIRE( stretch_left.start_iterate == left_point - distance);
    REQUIRE( stretch_left.end_iterate == anchor - 1);
    REQUIRE (stretch_left.stretch_description() == "50.25.256");

    REQUIRE( stretch_right.step == -1);
    REQUIRE( stretch_right.active == true);
    REQUIRE (stretch_right.start_iterate == right_point + distance);
    REQUIRE (stretch_right.end_iterate == anchor + 1);
    REQUIRE (stretch_right.stretch_description() == "300.25.256");


  }

  SECTION("Check valid stretch points") {

    int too_far_left = left_point - distance - 1;
    int too_far_right = right_point + distance + 1;

    int max_left = left_point - distance;
    int min_left = anchor - 1;

    int max_right = right_point + distance;
    int min_right = anchor + 1;

    REQUIRE(stretch_left.idx_to_copy_from(max_left) ==  ( left_point ));
    REQUIRE(stretch_left.idx_to_copy_from(min_left) == (min_left));
    REQUIRE(stretch_left.idx_to_copy_from(too_far_left) == too_far_left);

    REQUIRE(stretch_right.idx_to_copy_from(max_right) == ( right_point ));
    REQUIRE(stretch_right.idx_to_copy_from(min_right) == (min_right));
    REQUIRE(stretch_right.idx_to_copy_from(too_far_right) == too_far_right);
  }

  SECTION("Check invalid/no stretches") {

    // Use some stretch parameters that are out of bounds/result in no stretching

    // Don't want to do a strech
    Stretch_Info dont_stretch(-1,distance, anchor, dims);
    REQUIRE( dont_stretch.check_valid_points() == false);
    REQUIRE( dont_stretch.active == false);
    REQUIRE (dont_stretch.start_iterate == 0);
    REQUIRE (dont_stretch.end_iterate == dims);
    REQUIRE( dont_stretch.stretch_description() == "0.0.0");

    //New point just outisde bounds on right
    Stretch_Info stretch_too_big_pos(0, 1, anchor, dims);
    REQUIRE( stretch_too_big_pos.check_valid_points() == false);
    REQUIRE( stretch_too_big_pos.active == false);

    //New point just outside bounds on left
    Stretch_Info stretch_too_big_neg(dims, 1, anchor, dims);
    REQUIRE( stretch_too_big_neg.check_valid_points() == false);
    REQUIRE( stretch_too_big_neg.active == false);

  }
}

// TEST_CASE("Array indexing") {
//   Deform_Volume warper;
//   int dims = 5;
//   warper.dims = dims; //Length of x,y,z, dimension
//   //Setup
//   int i,j,k;
//   long idx;
//
//
//   SECTION("Indexing 'normal' values") {
//
//     for(i = 0; i < dims; i++) {
//       for(j = 0; j < dims; j++) {
//         for(k = 0; k < dims; k++) {
//
//           idx = warper.get_array_index(i, j, k);
//           REQUIRE (idx == (i * dims + j) * dims + k);
//
//
//         }
//       }
//     }
//
//     SECTION ("Indexing large dimension array") {
//       // Test for final elment of typical array size
//       int big_dim = 512;
//       warper.dims = big_dim;
//       int max_array_idx = big_dim - 1; // due to 0 indexing
//
//       idx = warper.get_array_index(max_array_idx, max_array_idx, max_array_idx);
//
//       REQUIRE(idx == big_dim*big_dim*big_dim-1);
//     }
//   }
//
//   SECTION("Trying to index values that don't exist") {
//
//     int too_big = dims+1;
//     REQUIRE (warper.get_array_index(too_big, too_big, too_big) == -1);
//     REQUIRE (warper.get_array_index(-1,-1,-1) == -1);
//
//   }
// }
//
//
//
// TEST_CASE("Neighbouring elements to first elment") {
//
//   Deform_Volume warper;
//   int dims = 5;
//   warper.dims = dims; //Length of x,y,z, dimension
//
//   long idx;
//     int expected_neighbours = 7;
//
//   // Check element 0
//   idx = 0;
//
//   vector<long> neighbours = warper.neighbouring_elements( idx);
//
//   long expected[] = {1, 5, 6, 25, 26, 30, 31};
//   vector<long> expected_results(expected, expected + sizeof(expected) / sizeof(long));
//
//   // Sort vector for consistent results
//   sort(neighbours.begin(), neighbours.end());
//
//   REQUIRE (neighbours.size() == expected_neighbours);
//   REQUIRE(neighbours == expected_results);
//
// }
//
//
// TEST_CASE("Neighbouring elements to last corner") {
//
//   Deform_Volume warper;
//   int dims = 5;
//   warper.dims = dims; //Length of x,y,z, dimension
//
//   long idx;
//
//   int expected_neighbours = 7;
//   idx = 124;
//
//   vector<long> neighbours = warper.neighbouring_elements(idx);
//
//   long expected[] = {93, 94, 98, 99, 118, 119, 123};
//   vector<long> expected_results(expected, expected + sizeof(expected) / sizeof(long));
//
//   // Sort vector for consistent results
//   sort(neighbours.begin(), neighbours.end());
//
//   REQUIRE (neighbours.size() == expected_neighbours);
//   REQUIRE(neighbours == expected_results);
//
//
// }
//
//
// TEST_CASE("Neighboruing elements to centre") {
//
//   Deform_Volume warper;
//   int dims = 5;
//   warper.dims = dims; //Length of x,y,z, dimension
//
//   long idx;
//
//   int expected_neighbours = 26;
//   idx = 62;
//
//   vector<long> neighbours = warper.neighbouring_elements(idx);
//
//   // Sort vector for consistent results
//   sort(neighbours.begin(), neighbours.end());
//
//   long expected[] = { 31, 32, 33, 36, 37, 38, 41, 42, 43,
//     56, 57, 58, 61, 63, 66, 67, 68,
//     81, 82, 83, 86, 87, 88, 91, 92, 93};
//
//     vector<long> expected_results(expected, expected + sizeof(expected) / sizeof(long));
//
//     REQUIRE (neighbours.size() == expected_neighbours);
//     REQUIRE(neighbours == expected_results);
//   }
//
//
// TEST_CASE ("Dilation") {
//
//   Deform_Volume warper;
//   int dims = 5;
//   warper.dims = dims; //Length of x,y,z, dimension
//
//     int total_elements = dims*dims*dims;
//
//     unsigned char test_input[dims*dims*dims];
//     warper.image_data = test_input;
//
//     int i,j,k;
//     int central_element = 62; // centre of 5x5x5
//     long idx;
//
//     unsigned char layer_0 = (unsigned char)0;
//     unsigned char layer_1 = (unsigned char)1;
//
//     for (i = 0; i < total_elements; i++) {
//       test_input[i] = layer_0;
//     }
//
//     SECTION ("Dilate central elment") {
//
//       // Set centrla elemnt to 1
//       test_input[central_element] = layer_1;
//
//       warper.dilate_layer(1, 1);
//
//       // Check some values that should not have changed
//       REQUIRE(test_input[0] == layer_0);
//       REQUIRE(test_input[2] == layer_0);
//       REQUIRE(test_input[dims*dims*dims-1] == layer_0);
//
//       // All these values should have changed
//       long expected[] = { 31, 32, 33, 36, 37, 38, 41, 42, 43,
//         56, 57, 58, 61, 63, 66, 67, 68,
//         81, 82, 83, 86, 87, 88, 91, 92, 93};
//         vector<long> expected_results(expected, expected + sizeof(expected) / sizeof(long));
//
//         long current_idx;
//
//         while (!expected_results.empty()) {
//
//           current_idx = expected_results.back();
//           REQUIRE ( test_input[current_idx] == layer_1);
//           expected_results.pop_back();
//
//         }
//
//       }
//
//     SECTION ("Dilate by 2 pixels") {
//       test_input[central_element] = layer_1;
//
//       // This should set all elments to 1
//       warper.dilate_layer(1, 2);
//
//       // Check all elements are 1
//       for (i = 0; i < total_elements; i++) {
//         REQUIRE( test_input[i] == layer_1);
//       }
//
//     }
//
//   }
