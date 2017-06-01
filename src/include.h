#ifndef INCLUDE_H
#define INCLUDE_H

//Input parameters that are read from the file
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>

#include <CGAL/Labeled_image_mesh_domain_3.h>
#include <CGAL/Mesh_domain_with_polyline_features_3.h>

#include <CGAL/make_mesh_3.h>
#include <CGAL/Image_3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>

// A change by James


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Labeled_image_mesh_domain_3<CGAL::Image_3,K> Mesh_domain;

typedef K::FT FT;
typedef K::Point_3 Point;
typedef K::Vector_3 Vector;

#endif
