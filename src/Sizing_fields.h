//Contains different sizing fields structures for different proposes: Spherical, Elliptical, planar gradient
// Refer to original CGAL documents if you would like to know more about sizing fields
// developed by Kirill Aristovich
// Example change by James H
//THIS CODE SUCKS!!!!!!!!
#include "CGAL_include.h"
#include "input_parameters.h"

using namespace std;

//Sizing field: Elliptical with electrodes
struct sizing_field_elliptic_electrodes
{
        //TODO: compiler error if below line is removed, but not entirely sure why
        // ::FT means the global value of FT is used.....
        typedef ::FT FT;

        Point origin;
        FT electrode_size; //element size near electrodes
        FT e_R; //electrode radius
        Points centres; //electrode centres
        FT fine_size, coarse_size;
        int preserve;

        FT ub_x,ub_y,ub_z;



        sizing_field_elliptic_electrodes(Point& origin_in, string path_electrode, Input params)
        {

          FILE *F;
          try { F=fopen(path_electrode.c_str(),"r");}
          catch (exception& e) { cout << e.what() << endl;}

          FT scale_xyz = 1/params.unit;

          coarse_size=params.options["cell_coarse_size_mm"];
          fine_size=params.options["cell_fine_size_mm"];
          preserve=int(params.options["elements_with_fine_sizing_fieldercentage"]);
          e_R=2*params.options["electrode_radius_mm"]; //2* to secure fit of the electrode
          electrode_size=params.options["cell_size_electrodes_mm"];//Planar gradient with electrodes -- size of the mesh near electrodes

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


      FT operator()(const Point& p, const int, const Index&) const

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
                Vector dist = p - origin;

                FT el = CGAL::sqrt((dist.x()/ub_x)*(dist.x()/ub_x)+(dist.y()/ub_y)*(dist.y()/ub_y)+(dist.z()/ub_z)*(dist.z()/ub_z));
                if (el>=1-FT(preserve)/100) out=fine_size;
                else out=fine_size + (coarse_size-fine_size)*(1-el);
                return out;
        }

};
