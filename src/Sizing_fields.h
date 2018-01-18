#ifndef SIZING_FIELD_H
#define SIZING_FIELD_H

#include "CGAL_include.h"
#include "input_parameters.h"


struct Sizing_field
{
        //TODO: compiler error if below line is removed, but not entirely sure why
        // ::FT means the global value of FT is used.....
        typedef ::FT FT;
        std::map<std::string, FT> options;
        Point origin;
        FT scale_xyz;
        FT ub_x,ub_y,ub_z;

        // LEss verbose names for some of the input paremters
        FT electrode_size; //element size near electrodes
        FT e_R; //electrode radius
        Points centres; //electrode centres
        FT fine_size, coarse_size;
        int preserve;
        int direction;

        Sizing_field(Point&, std::string, std::map<std::string, FT>);

      FT operator()(const Point&, const int, const Index&) const;


};

#endif
