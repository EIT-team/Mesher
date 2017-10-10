
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "warp_mesh.h"

#include <algorithm> // std::sort

using namespace std;

#define NDIMS 5

TEST_CASE("Array indexing") {

  //Setup
  int total_elements = NDIMS*NDIMS*NDIMS;

  int test_input[NDIMS*NDIMS*NDIMS];
  int i,j,k;

  long idx;

  for (i = 0; i < total_elements; i++) {
    test_input[i] = i;

      }


SECTION("Indexing 'normal' values") {

for(i = 0; i < NDIMS; i++) {
  for(j = 0; j < NDIMS; j++) {
    for(k = 0; k < NDIMS; k++) {

        idx = get_array_index(i, j, k, NDIMS);
        REQUIRE (test_input[idx] == idx);


    }
  }
}


}

 SECTION("Trying to index values that don't exist") {

   int too_big = NDIMS+1;
   REQUIRE (get_array_index(too_big, too_big, too_big, NDIMS) == -1);
   REQUIRE (get_array_index(-1,-1,-1, NDIMS) == -1);

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
