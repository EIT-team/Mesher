
#include "catch.hpp"

#include "stretch_info.h"

using namespace std;

TEST_CASE("Stretchingtest_stretching") {

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

    stretch_left.prepare_stretch(max_left);
    REQUIRE(stretch_left.idx_to_copy_from(max_left) ==  ( left_point ));

    stretch_left.prepare_stretch(min_left);
    REQUIRE(stretch_left.idx_to_copy_from(min_left) == (min_left));

    stretch_left.prepare_stretch(too_far_left);
    REQUIRE(stretch_left.idx_to_copy_from(too_far_left) == too_far_left);

    stretch_right.prepare_stretch(max_right);
    REQUIRE(stretch_right.idx_to_copy_from(max_right) == ( right_point ));

    stretch_right.prepare_stretch(min_right);
    REQUIRE(stretch_right.idx_to_copy_from(min_right) == (min_right));

    stretch_right.prepare_stretch(too_far_right);
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
