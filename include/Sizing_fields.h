//Contains different sizing fields structures for different proposes: Spherical, Elliptical, planar gradient
// Refer to original CGAL documents if you would like to know more about sizing fields
// developed by Kirill Aristovich
// Example change by James H
//THIS CODE SUCKS!!!!!!!!
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


// SIzing field: spherical
struct Spherical_sizing_field
{
  typedef ::FT FT;
  typedef Point Point_3;
  typedef Mesh_domain::Index Index;
  Point_3 origin;
  FT fine_size, coarse_size;

  Spherical_sizing_field(Point_3& p)
  {
	  origin = p;
  }
  FT operator()(const Point_3& p, const int, const Index&) const
  {
    FT sq_d_to_origin = CGAL::squared_distance(p, origin);
    return fine_size + coarse_size/(CGAL::sqrt(sq_d_to_origin)+1); 
  }
  
};

//Sizing field: Elliptical
struct sizing_field_elliptic
{
  typedef ::FT FT;
  typedef Point Point_3;
  typedef Vector Vector_3;
  typedef Mesh_domain::Index Index;
  Point_3 origin;

  FT ub_x,ub_y,ub_z;

  FT fine_size, coarse_size;
  int preserve;

  sizing_field_elliptic(Point_3& p)
  { 
	  origin=p;
	  ub_x=origin.x();
	  ub_y=origin.y();
	  ub_z=origin.z();
  }
  

  FT operator()(const Point_3& p, const int, const Index&) const
  {
	Vector_3 dist = p - origin;
	FT out;
	FT el = CGAL::sqrt((dist.x()/ub_x)*(dist.x()/ub_x)+(dist.y()/ub_y)*(dist.y()/ub_y)+(dist.z()/ub_z)*(dist.z()/ub_z));
	if (el>=1-FT(preserve)/100) out=fine_size;
	else out=fine_size + (coarse_size-fine_size)*(1-el);
	return out; 
  }
  
};

//Sizing field: Elliptical with electrodes
struct sizing_field_elliptic_electrodes
{
  typedef ::FT FT;
  typedef Point Point_3;
  typedef Vector Vector_3;
  typedef Mesh_domain::Index Index;
  typedef std::vector<Point>  Points;
  Point_3 origin;
  FT electrode_size; //element size near electrodes
  FT e_R; //electrode radius
  Points centres; //electrode centres
  FT fine_size, coarse_size; 
  int preserve;

  FT ub_x,ub_y,ub_z;

  sizing_field_elliptic_electrodes(Point_3& p, FILE* F, FT scale_x, FT scale_y, FT scale_z)
  { 
	  if (F == NULL) perror ("Error opening electrode file");
	  else {
		  while(!feof(F))
		  {
			  float x,y,z;
			  fscanf(F,"%f,%f,%f\n",&x,&y,&z);
			  Point_3 pt(x*scale_x,y*scale_y,z*scale_z);
			  centres.push_back(pt);		  }
	  }
	  origin=p;
	  ub_x=origin.x();
	  ub_y=origin.y();
	  ub_z=origin.z();
  }
  

  FT operator()(const Point_3& p, const int, const Index&) const
  {
	FT out;
	Points::const_iterator it;
	for (it=centres.begin(); it<centres.end();it++)
	{
	  Vector_3 pp=(p-*it);
	  if (pp.squared_length()<=e_R*e_R) 
	  {
		  out=electrode_size;
		  return out;
	  }
	}
	Vector_3 dist = p - origin;

	FT el = CGAL::sqrt((dist.x()/ub_x)*(dist.x()/ub_x)+(dist.y()/ub_y)*(dist.y()/ub_y)+(dist.z()/ub_z)*(dist.z()/ub_z));
	if (el>=1-FT(preserve)/100) out=fine_size;
	else out=fine_size + (coarse_size-fine_size)*(1-el);
	return out; 
  }
  
};


//Sizing field: planar gradient in one direction with electrodes
struct sizing_field_planar_electrodes
{
  typedef ::FT FT;
  typedef Point Point_3;
  typedef Vector Vector_3;
  typedef Mesh_domain::Index Index;
  typedef std::vector<Point>  Points;
  FT ub,height; //upper boundary, height of the plane
  int direction; //direction of the plane
  FT electrode_size; //element size near electrodes
  FT e_R; //electrode radius
  Points centres; //electrode centres
  FT fine_size, coarse_size; 
  int preserve;

  sizing_field_planar_electrodes(FT& h, int d, FT& b, FILE* F, FT scale)
  { 
	  
	  if (F == NULL) perror ("Error opening electrode file");
	  else {
		  while(!feof(F))
		  {
			  float x,y,z;
			  fscanf(F,"%f,%f,%f\n",&x,&y,&z);
			  Point_3 p(x*scale,y*scale,z*scale);
			  centres.push_back(p);			  
		  }
	  }
	  
	  direction=d;
	  height=h;
	  ub=b;
  }
  

  FT operator()(const Point_3& p, const int, const Index&) const
  {
	FT out;
	Points::const_iterator it;
	for (it=centres.begin(); it<centres.end();it++)
	{
	  Vector_3 pp=(p-*it);
	  if (pp.squared_length()<=e_R*e_R) 
	  {
		  out=electrode_size;
		  return out;
	  }
	}
	FT dist;
	if(direction==1)	dist=CGAL::abs(p.x()-height);
	else if(direction==2)	dist=CGAL::abs(p.y()-height);
	else if(direction==3)	dist=CGAL::abs(p.z()-height);
	else {return fine_size;}
	FT el = dist/ub;
	if (el<=FT(preserve)/100) out=fine_size;
	else out=fine_size + CGAL::abs((coarse_size-fine_size)*(el-FT(preserve)/100));
	
	return out; 
  }
  
};



struct sizing_field_jacobian
{
  typedef ::FT FT;
  typedef Point Point_3;
  typedef Vector Vector_3;
  typedef Mesh_domain::Index Index;
  typedef std::vector<Point>  Points;
  FT ub,height; //upper boundary, height of the plane
  int direction; //direction of the plane, will not be used here
  FT electrode_size; //element size near electrodes
  FT e_R; //electrode radius, now it will be used for bound
  Points centres; //electrode centres
  Point_3 origin,R;
  FT fine_size, coarse_size, scale; 
  int preserve;
  FT el_min,el_max;
  Index subdom;

  sizing_field_jacobian (FT& h, int& d, FT& b, FILE* F, FT sc, Point_3 orgin, Index& sub)
  { 
	 
	  if (F == NULL) perror ("Error opening electrode file");
	  else {
		  int start=0;
		  while(!feof(F))
		  {
			  float pt[3];
			  fscanf(F,"%f,%f,%f\n",&pt[0],&pt[1],&pt[2]);
//			  std::cout<<"\n"<<pt[0]<<"; "<<pt[1]<<"; "<<pt[2];
			  Point_3 p(pt[0]/scale,pt[1]/scale,pt[2]/scale);

			  if (start!=0)
			    {
				   el_min=(el_min>pt[2]) ? pt[2]:el_min;
				   el_max=(el_max<pt[2]) ? pt[2]:el_max;
				}
			  else 
				{
				   el_min=pt[2];
				   el_max=pt[2];
				   start=1;
				}
			  centres.push_back(p);			  
		  }
		  el_min-=0.2*(el_max-el_min);
		  el_max+=0.3*(el_max-el_min);
		 // std::cout<<el_min<<"         "<<el_max<<"\n";
	  }
	
	  R=Point_3 (0.2*origin.x(),0.3*origin.y(),0.6*origin.z());
	  height=h;
	  ub=b;
	  origin=origin;
	  direction=d;
	  scale=sc;
	  subdom=sub;
//	  std::cout<<"\n\n\n R1="<<R.x()<<"; R2="<<R.y()<<"; R3="<<R.z()<<"\n el_min3="<<el_min<<"\n el_max3="<<el_max<<"\n";
  }
  


  FT operator()(const Point_3& p, const int, const Index& vtx) const
  {
	FT out=coarse_size;
    Vector_3 pp=(p-origin);
	
	if (	p.z()>=(el_min)/scale && p.z()<=(el_max)/scale
		 && pp.y()<=0 && pp.x()<=0 
		 && (pp.x()/R.x())*(pp.x()/R.x()) +(pp.y()/R.y())*(pp.y()/R.y()) +(pp.z()/R.z())*(pp.z()/R.z())>=1
		 )
		 //&& vtx==subdom  )
	{
		  out=electrode_size;
		  return out;
	}
	
	FT dist;
	if(direction==1)	dist=CGAL::abs(p.x()-height);// plane 
	else if(direction==2)	dist=CGAL::abs(p.y()-height);
	else if(direction==3)	dist=CGAL::abs(p.z()-height);
	else {return fine_size;}
	FT el = dist/ub;
	if (el<=FT(preserve)/100) out=fine_size;
	else out=fine_size + CGAL::abs((coarse_size-fine_size)*(el-FT(preserve)/100));
	
	return out; 
  }
  
};

//Sizing field: planar gradient in one direction
struct sizing_field_planar
{
  typedef ::FT FT;
  typedef Point Point_3;
  typedef Vector Vector_3;
  typedef Mesh_domain::Index Index;
  FT ub,height;
  int direction;
  FT fine_size, coarse_size;
  int preserve;

  sizing_field_planar(FT& h, int d, FT& b)
  { 
	  direction=d;
	  height=h;
	  ub=b;
  }
  

  FT operator()(const Point_3& p, const int, const Index&) const
  {
	
	FT dist;
	if(direction==1)	dist=CGAL::abs(p.x()-height);
	else if(direction==2)	dist=CGAL::abs(p.y()-height);
	else if(direction==3)	dist=CGAL::abs(p.z()-height);
	else {return fine_size;}

	FT el = dist/ub;
	FT out;
	if (el<=FT(preserve)/100) out=fine_size;
	else out=fine_size + CGAL::abs((coarse_size-fine_size)*(el-FT(preserve)/100));
	return out; 
  }
  
};


