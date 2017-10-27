#ifndef FROM_MATLAB_H
#define FROM_MATLAB_H

#include "CGAL_include.h"
#include "include.h"

using namespace std;

Point centre_of_mesh(const C3t3&);

Point closest_element(const C3t3&, Point, int);

vector<Point> load_electrode_locations(FILE *, FT );

Point set_reference_electrode(const C3t3&);
;
Point set_ground_electrode(const C3t3&);


#endif
