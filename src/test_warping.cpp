
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "warp_mesh.h"

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
