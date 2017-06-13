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


// domain
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Labeled_image_mesh_domain_3<CGAL::Image_3,K> Mesh_domain;

// For inexact functions
typedef K::FT FT; //Field type number
typedef K::Point_3 Point;
typedef K::Vector_3 Vector;

// Polyline
typedef std::vector<Point>        Polyline_3;
typedef std::list<Polyline_3>       Polylines;

// Triangulation
typedef CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;

// Criteria
typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;

//! Iterators for Facets, Vertices and cells
typedef C3t3::Facets_in_complex_iterator Facet_iterator;
typedef C3t3::Cells_in_complex_iterator Cell_iterator;
typedef Tr::Finite_vertices_iterator Finite_vertices_iterator;
typedef Tr::Vertex_handle Vertex_handle;

//! Point
typedef Tr::Point Point_3;

//! Map generators (connectivity and domains)
typedef CGAL::Mesh_3::Medit_pmap_generator<C3t3,true,true> Generator;
typedef  Generator::Cell_pmap Cell_pmap;
typedef  Generator::Facet_pmap Facet_pmap;
typedef  Generator::Facet_pmap_twice Facet_pmap_twice;
typedef Generator::Vertex_pmap Vertex_pmap;

//For Sizing field
typedef Mesh_domain::Index Index;
typedef std::vector<Point>  Points;


#endif
