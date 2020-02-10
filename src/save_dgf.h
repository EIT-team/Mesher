#ifndef SAVE_DGF_H
#define SAVE_DGF_H

#include "CGAL_include.h"
#include "input_parameters.h"
#include "include.h"

void save_as_dgf(const C3t3&, std::map<std::string, FT>, std::string);
void save_electrodes(Points, std::string);
void save_deformation_info(std::string, std::string);
void save_parameters(std::map<std::string, std::string>, std::string, std::string);
void write_centres(C3t3&, std::string);
void save_matlab(const C3t3&, std::map<std::string, FT>, std::string);

#endif
