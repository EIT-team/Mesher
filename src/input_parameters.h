#ifndef INPUT_PARAMETERS_H
#define INPUT_PARAMETERS_H

#include "CGAL_include.h"

struct Input
{

        int direction, height; //direction and height of the starting plane for planar sizer
        FT unit; // scaling units - mm per pixel (note, that main sizing conversion comes directly from image)
        std::map<std::string, FT> options;
        //constructor contains defaults just in case
        Input()
        {
                // fa=30; cre=3; fs=3; fd=3; cs=5; ccs=10;
                // if_lloyd=0; if_odt=0; if_exude=1; if_perturb=1;
                // pres=0; time_lim=0;
        }

        void load_file_idx(char*);



};

#endif
