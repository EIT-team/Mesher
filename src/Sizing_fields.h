#ifndef SIZING_FIELD_H
#define SIZING_FIELD_H

#include "CGAL_include.h"
#include "input_parameters.h"


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


        sizing_field_elliptic_electrodes(Point&, std::string, Input);

      FT operator()(const Point&, const int, const Index&) const;


};

#endif
