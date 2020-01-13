#ifndef INPUT_PARAMETERS_H
#define INPUT_PARAMETERS_H

#include "CGAL_include.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::map<std::string, FT> read_params_from_file(char *);
std::vector<std::vector<double>> load_deformations(const char *);
int check_distances_ok(double, double, double);

#endif
