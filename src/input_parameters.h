#ifndef INPUT_PARAMETERS_H
#define INPUT_PARAMETERS_H

#include "CGAL_include.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::map<std::string, FT> read_params_from_file(char *);
void validate_params(std::map<std::string, FT>);

#endif
