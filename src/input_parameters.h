#ifndef INPUT_PARAMETERS_H
#define INPUT_PARAMETERS_H

#include "CGAL_include.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::map<std::string, FT>  load_file_idx(char*);
std::vector< std::vector<double> > load_deformations(const char*);

#endif