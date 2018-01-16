#ifndef SIZING_FIELD_H
#define SIZING_FIELD_H

#include "CGAL_include.h"
#include "input_parameters.h"


struct Sizing_field
{
        //TODO: compiler error if below line is removed, but not entirely sure why
        // ::FT means the global value of FT is used.....
        typedef ::FT FT;

        //TODO: store the whole parameter structire here, rather than reassigning to individual vars?
        Point origin;
        FT electrode_size; //element size near electrodes
        FT e_R; //electrode radius
        Points centres; //electrode centres
        FT fine_size, coarse_size;
        int preserve;
        int do_planar_refinement; // Whether to do planar refinement
        int direction = 0, height = 0, upper_bound = 0; // Planar refinement parameters
        FT ub_x,ub_y,ub_z;


        Sizing_field(Point&, std::string, Input);

      FT operator()(const Point&, const int, const Index&) const;


};

#endif
