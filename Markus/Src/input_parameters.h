
//Input parameters that are read from the file
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>

#include <CGAL/Labeled_image_mesh_domain_3.h>
#include <CGAL/Mesh_domain_with_polyline_features_3.h>

#include <CGAL/make_mesh_3.h>
#include <CGAL/Image_3.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Labeled_image_mesh_domain_3<CGAL::Image_3,K> Mesh_domain;

typedef K::FT FT;
typedef K::Point_3 Point;
typedef K::Vector_3 Vector;

struct Input
{
  int fa,cre; //facet angle, and cell ratio
  float fd,cs,ccs; // facet size, facet distance, cell fine size, cell coarse size
  float e_size, e_R; //size of the cell near electrode, radius of the electrode
  int if_lloyd,if_odt,if_exude, if_perturb; //logicals for optimisation
  int pres; //preserve element percentage
  int direction, height; //direction and height of the starting plane for planar sizer
 // int size_type;//type of sizing field
  int time_lim; // time limit for each optimisation
  char f_name[100]; //just some rubbish collector
  float unit; // scaling units - mm per pixel (note, that main sizing conversion comes directly from image)
  //constructor contains defaults just in case
  Input()
  {
	 // fa=30; cre=3; fs=3; fd=3; cs=5; ccs=10; 
	 // if_lloyd=0; if_odt=0; if_exude=1; if_perturb=1;
	 // pres=0; time_lim=0;
  }

  void load_file_planar()
  {
	  FILE* F=fopen("input_planar.txt","r");
	  if (F == NULL) perror ("Error opening file");
	  else {
	  std::cout<<"\n Reading the input file: ";
	  fgets(f_name, 100, F);
	  fgets(f_name, 100, F);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&unit);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&fa);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&fd);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&cre);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&cs);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&ccs);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&e_R); 
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&e_size);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&direction);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&height);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&pres);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&if_lloyd);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&if_odt);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&if_exude);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&if_perturb);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&time_lim);
	  fclose(F);
	  std::cout<<"\n-facet angle = "<<fa<<"\n-faced distance= "<<fd<<"\n-cell radius edge ratio = "
		  <<cre<<"\n-cell fine size = "<<cs<<"\n-cell coarse size = "<<ccs
		  <<"\n-preserve elements with fine size (perc. from the border)= "<<pres<<"%\n"
		  <<"\n-electrode radius = "<<e_R<<"\n-mesh size near the electrode = "<<e_size;
	  std::cout<<"\n-make lloyd = "<<if_lloyd<<"\n-make odt = "<<if_odt<<"\n-make exude = "<<if_exude<<"\n-make perturb = "<<if_perturb<<"\n-time limit = "<<time_lim;
	  // rescale
	  fd/=unit; cs/=unit; ccs/=unit; e_size/=unit; e_R/=unit; 
	  }
  }

   void load_file_elliptic()
  {
	  FILE* F=fopen("input_elliptic.txt","r");
	  if (F == NULL) perror ("Error opening file");
	  else {
	  std::cout<<"\n Reading the input file: ";
	  fgets(f_name, 100, F);
	  fgets(f_name, 100, F);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&unit);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&fa);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&fd);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&cre);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&cs);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&ccs);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&pres);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&if_lloyd);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&if_odt);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&if_exude);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&if_perturb);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&time_lim);
	  fclose(F);
	  std::cout<<"\n-facet angle = "<<fa<<"\n-faced distance= "<<fd<<"\n-cell radius edge ratio = "
		  <<cre<<"\n-cell fine size = "<<cs<<"\n-cell coarse size = "<<ccs
		  <<"\n-preserve elements with fine size (perc. from the border)= "<<pres<<"%\n";
	  std::cout<<"\n-make lloyd = "<<if_lloyd<<"\n-make odt = "<<if_odt<<"\n-make exude = "<<if_exude<<"\n-make perturb = "<<if_perturb<<"\n-time limit = "<<time_lim;
	  // rescale
	  fd/=unit; cs/=unit; ccs/=unit; 
	  }
  }

   void load_file_elliptic_electrodes()
  {
	  FILE* F=fopen("input_elliptic_electrodes.txt","r");
	  if (F == NULL) perror ("Error opening file");
	  else {
	  std::cout<<"\n Reading the input file: ";
	  fgets(f_name, 100, F);
	  fgets(f_name, 100, F);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&unit);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&fa);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&fd);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&cre);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&cs);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&ccs);
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&e_R); 
	  fgets(f_name, 100, F);
	  fscanf(F,"%f\n",&e_size);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&pres);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&if_lloyd);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&if_odt);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&if_exude);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&if_perturb);
	  fgets(f_name, 100, F);
	  fscanf(F,"%d\n",&time_lim);
	  fclose(F);
	  std::cout<<"\n-facet angle = "<<fa<<"\n-facet distance = "<<fd<<"\n-cell radius edge ratio = "
		  <<cre<<"\n-cell fine size = "<<cs<<"\n-cell coarse size = "<<ccs
		  <<"\n-preserve elements with fine size (perc. from the border)= "<<pres<<"%\n"<<"\n-electrode radius = "<<e_R<<"\n-mesh size near the electrode = "<<e_size;;
	  std::cout<<"\n-make lloyd = "<<if_lloyd<<"\n-make odt = "<<if_odt<<"\n-make exude = "<<if_exude<<"\n-make perturb = "<<if_perturb<<"\n-time limit = "<<time_lim;
	  // rescale
	  fd/=unit; cs/=unit; ccs/=unit; e_size/=unit; e_R/=unit;
	  }
   }
};