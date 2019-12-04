#ifndef SIZING_FIELD_H
#define SIZING_FIELD_H

#include "CGAL_include.h"
#include "input_parameters.h"

struct Sizing_field
{
  // ::FT means the global value of FT is used. We need to do this to avoid
  // Compiler error, but not entirely sure why...
  typedef ::FT FT;
  std::map<std::string, FT> options;
  Point origin;
  FT scale_xyz;
  FT ub_x, ub_y, ub_z;

  // Less verbose names for some of the input paremters
  FT elem_size_electrodes;   //element size near electrodes
  FT e_R;                    //electrode radius
  Points centres;            //electrode centres
  FT fine_size, coarse_size; // size of fine/coarse elements
  int fine_size_percentage;  // percentage of elements with fine size

  Sizing_field(Point &, std::string, std::map<std::string, FT>);

  FT operator()(const Point &, const int, const Index &) const;
};

#endif
