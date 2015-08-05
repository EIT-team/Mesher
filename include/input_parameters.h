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

struct Input
{
  //int fa,cre; //facet angle, and cell ratio
  //FT fd,cs,ccs; // facet size, facet distance, cell fine size, cell coarse size
  //FT e_size, e_R; //size of the cell near electrode, radius of the electrode
  //int if_lloyd,if_odt,if_exude, if_perturb; //logicals for optimisation
  //int pres; //preserve element percentage
  int direction, height; //direction and height of the starting plane for planar sizer
 // int size_type;//type of sizing field
  //int time_lim; // time limit for each optimisation
  //char f_name[100]; //just some rubbish collector
  FT unit; // scaling units - mm per pixel (note, that main sizing conversion comes directly from image)
  std::map<std::string, FT> options;
  //constructor contains defaults just in case
  Input()
  {
	 // fa=30; cre=3; fs=3; fd=3; cs=5; ccs=10; 
	 // if_lloyd=0; if_odt=0; if_exude=1; if_perturb=1;
	 // pres=0; time_lim=0;
  }

  void load_file_idx(char* file_name_input)
  {
	std::ifstream cfgfile(file_name_input, std::ifstream::in); // = std::ifstream::open(file_name_input);
	if (cfgfile == NULL) perror ("\n Error opening input parameters file");
	 
	std::string id, eq;
	FT val;
	 

	while(cfgfile >> id >> eq >> val)
		{
		  if (id[0] == '#') continue;  // skip comments
		  if (eq != "=") throw std::runtime_error("Input parameters file parse error: has to be variable[space]=[space]value");

		  options[id] = val;
		  std::cout << id <<" = "<< options[id] << "\n";
		}

	height = int (options["starting_point_finest_mm"]);
	unit = options["pixel_scale_mm"];
	direction = int(options["planar_sizing_direction_XYZ"]);
	options["facet_distance_mm"]/=unit; 
	options["cell_fine_size_mm"]/=unit; 
	options["cell_coarse_size_mm"]/=unit; 
	options["cell_size_electrodes_mm"]/=unit; 
	options["electrode_radius_mm"]/=unit; 


  }

  
};
