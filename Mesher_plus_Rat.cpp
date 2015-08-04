#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>

#include <CGAL/Labeled_image_mesh_domain_3.h>
#include <CGAL/Mesh_domain_with_polyline_features_3.h>

#include <CGAL/make_mesh_3.h>
#include <CGAL/Image_3.h>

//Sizing fields
#include <Sizing_fields.h> 
//Input parameters
#include <input_parameters.h>
#include <Matlab_save.h>


// Domain
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Labeled_image_mesh_domain_3<CGAL::Image_3,K> Mesh_domain;


//For inexact functions
typedef K::FT FT;
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
//typedef CGAL::Mesh_constant_domain_field_3<Mesh_domain::R,
                                           //Mesh_domain::Index> Sizing_field;


// To avoid verbose function and named parameters call
using namespace CGAL::parameters;
using namespace std;

int main(int argc, char* argv[])
{
 
  


  cout <<	" Input image file: " << argv[1] 
  <<		"\n Electrode position file: " << argv[2] 
  <<		"\n Input parameters file: " << argv[3] << "\n";

  // Loads image
  CGAL::Image_3 image;

  // Read input file with parameters
  Input p;
  p.load_file_idx(argv[3]);
  

  // Reading image file
  std::cout<<"\n Reading the Image file... ";
  image.read(argv[1]);
  
  // Domain
  Mesh_domain domain(image);
  
  //Define Sizing field
  Point origin(image.vx () * image.xdim ()/2,
		   image.vy () * image.ydim ()/2, 
		   image.vz () * image.zdim ()/2); //origin

  

  FT h, ub; 
  if(p.options["planar_sizing_direction_XYZ"]==1) {h=p.height*image.vx (); ub=image.vx () * image.xdim ();}
  else if(p.options["planar_sizing_direction_XYZ"]==2) {h=p.height*image.vy (); ub=image.vy () * image.ydim ();}
  else if(p.options["planar_sizing_direction_XYZ"]==3) {h=p.height*image.vz (); ub=image.vz () * image.zdim ();}
  else {std::cout<<"wrong direction index, sizer will use constant fine size everywhere"; h=1;ub=1;}

  FILE *F;
  try { F=fopen(argv[2],"r");}
  catch (exception& e) { cout << e.what() << endl;}
  
 
  Mesh_domain::Index sub = domain.index_from_subdomain_index(2);
  sizing_field_jacobian size_p (h,p.direction,ub,F,p.options["pixel_scale_mm"], origin,sub);
  //sizing_field_jacobian size_p_coarse (h,p.direction,ub,F,p.unit, or,sub);
  if (F!=NULL) fclose(F);
  //sizing_field_planar_electrodes size_p (h,p.direction,ub);

  size_p.coarse_size=p.options["cell_coarse_size_mm"];
  size_p.fine_size=p.options["cell_fine_size_mm"];
  size_p.preserve=int(p.options["elements_with_fine_size_percentage"]);
  size_p.e_R=3*p.options["electrode_radius_mm"]; //2* to secure fit of the electrode
  size_p.electrode_size=p.options["cell_size_electrodes_mm"];//Planar gradient with electrodes -- size of the mesh near electrodes

  //For initial smoothing we mesh it with coasrser mesh:
  //size_p_coarse.fine_size=5*p.cs;
  //size_p_coarse.coarse_size=size_p_coarse.fine_size;
 // size_p_coarse.preserve=0;
 // size_p_coarse.e_R=0;
 // size_p_coarse.electrode_size=size_p_coarse.fine_size;
     
  // Mesh criteria: faces and cells
  Mesh_criteria criteria(facet_angle=p.fa, facet_size=size_p, facet_distance=p.fd,
                         cell_radius_edge_ratio=p.cre, cell_size=size_p);
  // For smoothing
 // Mesh_criteria criteria_coarse(facet_angle=p.fa, facet_size=size_p_coarse, facet_distance=p.fd,
    //                     cell_radius_edge_ratio=p.cre, cell_size=size_p_coarse);
  

  // Meshing
  std::cout<<"\n Meshing with initial mesh...";
  C3t3 c3t3;
  	  //try { //Do not need this anymore in x64 version
		  
  c3t3= CGAL::make_mesh_3<C3t3>(domain, criteria, CGAL::parameters::features(domain), 
						CGAL::parameters::no_lloyd(), CGAL::parameters::no_odt(), 
						CGAL::parameters::no_perturb(),CGAL::parameters::no_exude()); 
	  //}
  
 // std::cout<<"\n Smoothing and sizing the mesh...";

 // CGAL::refine_mesh_3<C3t3>(c3t3, domain, criteria, CGAL::parameters::no_lloyd(), CGAL::parameters::no_odt(), 
//						CGAL::parameters::no_perturb(),CGAL::parameters::no_exude()); 
	 

		    
//Optimisation
  std::cout<<"\n Optimising: ";
  if (int(p.options["perturb_opt"])==1) {std::cout<<"\n Perturb... "; CGAL::perturb_mesh_3(c3t3, domain,sliver_bound=10, time_limit=p.options["time_limit_sec"]);}
  if (int(p.options["lloyd_opt"])==1)  {std::cout<<"\n Lloyd... ";CGAL::lloyd_optimize_mesh_3(c3t3, domain, time_limit=p.options["time_limit_sec"]);}
  if (int(p.options["odt_opt"])==1)  {std::cout<<"\n ODT... "; CGAL::odt_optimize_mesh_3(c3t3, domain, time_limit=p.options["time_limit_sec"]);}
  if (int(p.options["exude_opt"])==1)  {std::cout<<"\n Exude... "; CGAL::exude_mesh_3(c3t3, sliver_bound=10, time_limit=p.options["time_limit_sec"]);}
 
  // Output
  
  /*std::ofstream medit_file("out.mesh");
  c3t3.output_to_medit(medit_file);
  medit_file.close();*/
  	
  
  
  //matlab output
  std::cout<<"\n Saving the mesh into matlab file... ";
  int save=save_matlab(c3t3,p);
   
  //all done
  std::cout<<"\n ALL DONE, press any key! :)";
  getchar();

  return 0;
}
