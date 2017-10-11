
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "warp_mesh.h"

#include <algorithm> // std::sort

using namespace std;


TEST_CASE("Array indexing") {

  //Setup
  int dims = 5; //Length of x,y,z, dimension
  int i,j,k;
  long idx;


  SECTION("Indexing 'normal' values") {

    for(i = 0; i < dims; i++) {
      for(j = 0; j < dims; j++) {
        for(k = 0; k < dims; k++) {

          idx = get_array_index(i, j, k, dims);
          REQUIRE (idx == (i * dims + j) * dims + k);


        }
      }
    }

    SECTION ("Indexing large dimension array") {
      // Test for final elment of typical array size
      int big_dim = 512;
      int max_array_idx = big_dim - 1; // due to 0 indexing

      idx = get_array_index(max_array_idx, max_array_idx, max_array_idx, 512);

      REQUIRE(idx == big_dim*big_dim*big_dim-1);
    }
  }

  SECTION("Trying to index values that don't exist") {

    int too_big = dims+1;
    REQUIRE (get_array_index(too_big, too_big, too_big, dims) == -1);
    REQUIRE (get_array_index(-1,-1,-1, dims) == -1);

  }
}



TEST_CASE("Neighbouring elements to first elment") {

  long idx;
  int dims = 5;
  int expected_neighbours = 7;

  // Check element 0
  idx = 0;

  vector<long> neighbours = neighbouring_elements( idx, dims);

  long expected[] = {1, 5, 6, 25, 26, 30, 31};
  vector<long> expected_results(expected, expected + sizeof(expected) / sizeof(long));

  // Sort vector for consistent results
  sort(neighbours.begin(), neighbours.end());

  REQUIRE (neighbours.size() == 7);
  REQUIRE(neighbours == expected_results);

}


TEST_CASE("Neighbouring elements to last corner") {

  long idx;
  int dims = 5;
  int expected_neighbours = 7;
  idx = 124;

  vector<long> neighbours = neighbouring_elements(idx, dims);

  long expected[] = {93, 94, 98, 99, 118, 119, 123};
  vector<long> expected_results(expected, expected + sizeof(expected) / sizeof(long));

  // Sort vector for consistent results
  sort(neighbours.begin(), neighbours.end());

  REQUIRE (neighbours.size() == 7);
  REQUIRE(neighbours == expected_results);


}


TEST_CASE("Neighboruing elements to centre") {

  long idx;
  int dims = 5;
  int expected_neighbours = 7;
  idx = 62;

  vector<long> neighbours = neighbouring_elements(idx, dims);

  // Sort vector for consistent results
  sort(neighbours.begin(), neighbours.end());

  long expected[] = { 31, 32, 33, 36, 37, 38, 41, 42, 43,
    56, 57, 58, 61, 63, 66, 67, 68,
    81, 82, 83, 86, 87, 88, 91, 92, 93};

    vector<long> expected_results(expected, expected + sizeof(expected) / sizeof(long));

    REQUIRE (neighbours.size() == 26);
    REQUIRE(neighbours == expected_results);
  }


TEST_CASE ("Dilation") {

    int dims = 5; //Length of x,y,z, dimension
    int total_elements = dims*dims*dims;

    unsigned char test_input[dims*dims*dims];
    int i,j,k;
    int central_element = 62; // centre of 5x5x5
    long idx;

    unsigned char layer_0 = (unsigned char)0;
    unsigned char layer_1 = (unsigned char)1;

    for (i = 0; i < total_elements; i++) {
      test_input[i] = layer_0;
    }

    SECTION ("Dilate central elment") {

      // Set centrla elemnt to 1
      test_input[central_element] = layer_1;

      dilate_layer(test_input, 1, 1, dims);

      // Check some values that should not have changed
      REQUIRE(test_input[0] == layer_0);
      REQUIRE(test_input[2] == layer_0);
      REQUIRE(test_input[dims*dims*dims-1] == layer_0);

      // All these values should have changed
      long expected[] = { 31, 32, 33, 36, 37, 38, 41, 42, 43,
        56, 57, 58, 61, 63, 66, 67, 68,
        81, 82, 83, 86, 87, 88, 91, 92, 93};
        vector<long> expected_results(expected, expected + sizeof(expected) / sizeof(long));

        long current_idx;

        while (!expected_results.empty()) {

          current_idx = expected_results.back();
          REQUIRE ( test_input[current_idx] == layer_1);
          expected_results.pop_back();

        }

      }

    SECTION ("Dilate by 2 pixels") {
      test_input[central_element] = layer_1;

      // This should set all elments to 1
      dilate_layer(test_input, 1, 2, dims);

      // Check all elements are 1
      for (i = 0; i < total_elements; i++) {
        REQUIRE( test_input[i] == layer_1);
      }

    }

  }
