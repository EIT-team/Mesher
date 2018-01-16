//Contains different sizing fields structures for different proposes: Spherical, Elliptical, planar gradient
// Refer to original CGAL documents if you would like to know more about sizing fields
// developed by Kirill Aristovich
// Example change by James H
//THIS CODE SUCKS!!!!!!!!

#include "Sizing_fields.h"
using namespace std;

Sizing_field::Sizing_field(Point& origin_in, string path_electrode, Input params)
{

  FILE *F;
  try { F=fopen(path_electrode.c_str(),"r");}
  catch (exception& e) { cout << e.what() << endl;}

  FT scale_xyz = 1/params.unit;

  coarse_size=params.options["cell_coarse_size_mm"];
  fine_size=params.options["cell_fine_size_mm"];
  preserve=int(params.options["elements_with_fine_sizing_field_percentage"]);
  e_R=2*params.options["electrode_radius_mm"]; //2* to secure fit of the electrode
  electrode_size=params.options["cell_size_electrodes_mm"];//Planar gradient with electrodes -- size of the mesh near electrodes
  do_planar_refinement = params.options["planar_refinement"];

if (do_planar_refinement) {
  height = params.options["height"];
  upper_bound = params.options["upper_bound"];
  direction = params.options["direction"];

}

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
  origin=origin_in;
  ub_x=origin.x();
  ub_y=origin.y();
  ub_z=origin.z();

  if (F!=NULL) fclose(F);

}


FT Sizing_field::operator()(const Point& p, const int, const Index&) const

{

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

  if (do_planar_refinement == 1)
  {
    FT dist;

    if(direction == 1)
    dist=CGAL::abs(p.x()- height);

    else if(direction == 2)
    dist=CGAL::abs(p.y()- height);

    else if(direction == 3)
    dist=CGAL::abs(p.z()- height);

    else {return fine_size;}

    FT el = dist / upper_bound;

    if (el<=FT(preserve)/100)
     out=fine_size;

    else out = fine_size + CGAL::abs((coarse_size-fine_size)*(el-FT(preserve)/100));

  }

  else { //do elliptical refinement_type


    // Cartersian distance from centre of the mesh
    FT distance = CGAL::sqrt( CGAL::squared_distance(p, origin));

    if (distance>=1-FT(preserve)/100) out=fine_size;
    else out=fine_size + (coarse_size-fine_size)*(1-distance);


  }
  return out;
}
