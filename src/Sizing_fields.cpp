//Contains different sizing fields structures for different proposes: Spherical, Elliptical, planar gradient
// Refer to original CGAL documents if you would like to know more about sizing fields
// developed by Kirill Aristovich
// Example change by James H
//THIS CODE SUCKS!!!!!!!!

#include "Sizing_fields.h"
using namespace std;

Sizing_field::Sizing_field(Point& origin_in, string path_electrode, std::map<std::string, FT> opts)
{

  FILE *F;
  try { F=fopen(path_electrode.c_str(),"r");}
  catch (exception& e) { cout << e.what() << endl;}

  options = opts;
  origin = origin_in;

  ub_x=origin.x();
  ub_y=origin.y();
  ub_z=origin.z();

  FT scale_xyz = 1/options["pixel_scale_mm"];

  // Reallocate some parameters to have less verbose names
  coarse_size = options["cell_coarse_size_mm"];
  fine_size = options["cell_fine_size_mm"];
  preserve = int(options["elements_with_fine_sizing_field_percentage"]);
  e_R = 2 * options["electrode_radius_mm"]; //2* to secure fit of the electrode
  electrode_size = options["cell_size_electrodes_mm"];//Planar gradient with electrodes -- size of the mesh near electrodes

  if (options["planar_refinement"])

  {

    if( options["planar_xyz"]==1 ) {
      options["height"]  *= options["vx"];
      options["upper_bound"]  = options["vx"] * options["xdim"];
    }

    else if( options["planar_xyz"]==2 ) {
      options["height"]  *= options["vy"];
      options["upper_bound"]  = options["vy"] * options["ydim"];
    }

    else if( options["planar_xyz"]==3 ) {
      options["height"]  *= options["vz"];
      options["upper_bound"]  = options["vz"] * options["zdim"];
    }

    else { // Invalid parameter passed
      cout << "Invalid planar direction specified, should be 1, 2 or 3" << endl;
      exit(0);
    }

  }

  // Load electrode positions
  if (F == NULL) perror ("Error opening electrode file");
  else {
    while(!feof(F))
    {
      float x,y,z;
      fscanf(F,"%f,%f,%f\n",&x,&y,&z);
      Point pt(x*scale_xyz,y*scale_xyz,z*scale_xyz);
      centres.push_back(pt);
    }
  }

  if (F!=NULL) fclose(F);

}


FT Sizing_field::operator()(const Point& p, const int, const Index&) const

{
  // Mesh the electrodes
  FT out;
  Points::const_iterator it;
  for (it=centres.begin(); it<centres.end(); it++)
  {
    Vector pp=(p-*it);
    if (pp.squared_length()<=e_R*e_R)
    {

      out=electrode_size;
      return out;
    }
  }



  double distance, distance_x, distance_y, distance_z, distance_max;
  // Do some additional refienments if turned on in parameter file
  // Need to use MAP.at("x") rather than MAP["x"] to be const safe

  if (options.at("sphere_refinement") ) {
    // Refine a sphere around a specificed point.

    Point sphere_centre(  options.at("sphere_centre_x"),
                          options.at("sphere_centre_y"),
                          options.at("sphere_centre_z"));


    distance = CGAL::sqrt( CGAL::squared_distance(p, sphere_centre) );
	


    if ( distance < FT(options.at("sphere_radius")) ) {

      out = options.at("sphere_cell_size");
    }

    else {
      out = coarse_size;
    }

  }

  if (options.at("square_refinement") ) {

  std::vector<FT> out_refine;
  FT sum;
  FT out_ref;
    // Refine a sphere around a specificed point.

    Point square_centre(  options.at("square_centre_x"),
                          options.at("square_centre_y"),
                          options.at("square_centre_z"));

    distance_x = CGAL::abs(p.x()- square_centre.x());
	distance_y = CGAL::abs(p.y()- square_centre.y());
	distance_z = CGAL::abs(p.z()- square_centre.z());
	distance = CGAL::sqrt( CGAL::squared_distance(p, square_centre) );
	distance_max = 2*scale_xyz;


    if ( distance_x > FT(options.at("square_x_extent")) && distance_x < FT(options.at("square_x_extent")) + distance_max )  {
		distance = distance_x - FT(options.at("square_x_extent"));
		out_ref = fine_size + (coarse_size - fine_size)*(distance/distance_max);
		out_refine.push_back(out_ref);
	}

	if ( distance_y > FT(options.at("square_y_extent")) && distance_y < FT(options.at("square_y_extent")) + distance_max )  {
		distance = distance_y - FT(options.at("square_y_extent"));
		out_ref = fine_size + (coarse_size - fine_size)*(distance/distance_max);
		out_refine.push_back(out_ref);
	}

	if ( distance_z > FT(options.at("square_z_extent")) && distance_x < FT(options.at("square_z_extent")) + distance_max )  {
		distance = distance_x - FT(options.at("square_z_extent"));
		out_ref = fine_size + (coarse_size - fine_size)*(distance/distance_max);
		out_refine.push_back(out_ref);
	}

	for (int i; i < out_refine.size(); i++){
	
		sum = sum + out_refine(i);
	}

	out = sum/out_refine.size();

	
	else if ( distance_x < FT(options.at("square_x_extent")) && distance_y < FT(options.at("square_y_extent")) && distance_z < FT(options.at("square_z_extent"))) {

      out = options.at("square_cell_size");
    }
	
	else {
		out = coarse_size;
	}

 
  }

  else if (options.at("planar_refinement"))   {

    if(options.at("planar_xyz") == 1) {
      distance=CGAL::abs(p.x()- options.at("height"));
	}

    else if(options.at("planar_xyz") == 2) {
      distance=CGAL::abs(p.y()- options.at("height"));
    }

    else if(options.at("planar_xyz") == 3) {
      distance=CGAL::abs(p.z()- options.at("height"));
    }

	 else {
      return fine_size;
    }

    double dist_percentage = distance / double(options.at("upper_bound")) ;

    if (dist_percentage <= (preserve)/100.0) {
      out=fine_size;
    }


    else  {
      out = fine_size +
       CGAL::abs((coarse_size-fine_size)*(dist_percentage - (preserve)/100.0));
	}

  }

  else { //refine centre of mesh more than outside

    //TODO: Not sure if this algorithm is legit
    // Cartersian distance from centre of the mesh
    Vector distance_eliptic = p - origin;
    FT distance_percent = CGAL::sqrt(     (distance_eliptic.x()/origin.x()) * (distance_eliptic.x()/origin.x()) +
                                          (distance_eliptic.y()/origin.y()) * (distance_eliptic.y()/origin.y()) +
                                          (distance_eliptic.z()/origin.z()) * (distance_eliptic.z()/origin.z())  );

    if (distance_percent >= 1-FT(preserve)/100) {
      out=fine_size + (coarse_size-fine_size) * (1-distance_percent);
    }

    else {
      out=fine_size;
    }


  }

   
   double distance_ref, distance_max;
   FT out_ref;
   FT out_min;
   std::vector<FT> out_options;
   out_options.push_back(out);
   
  for (it=centres.begin(); it<centres.end(); it++)
  {
    Vector pp=(p-*it);
	distance_ref = CGAL::sqrt( pp.squared_length() )-e_R;
	distance_max = 2*e_R;

		if ( distance_ref <= distance_max)
			{
				out_ref = electrode_size + (fine_size - electrode_size)*(distance_ref/distance_max);
				out_options.push_back(out_ref);
			}
 }

 //for (it = out_options.begin(); it < out_options.end(); it++)
 //{
   //if (out_options.)
 //}


  (out_min) = *std::min_element(out_options.begin(), out_options.end());
  out = out_min;

  return out;
}
