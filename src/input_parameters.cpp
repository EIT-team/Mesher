#include "input_parameters.h"

        void Input::load_file_idx(char* file_name_input)
        {
                std::ifstream cfgfile(file_name_input, std::ifstream::in); // = std::ifstream::open(file_name_input);
                if (!cfgfile) perror ("\n Error opening input parameters file");

                std::string id, eq;
                FT val;


                while(cfgfile >> id >> eq >> val)
                {
                        if (id[0] == '#') continue; // skip comments
                        if (eq != "=") throw std::runtime_error("Input parameters file parse error: has to be variable[space]=[space]value");

                        options[id] = val;
                        std::cout << id <<" = "<< options[id] << "\n";
                }

                height = int (options["starting_point_finest_mm"]);
                unit = options["pixel_scale_mm"];
                direction = int(options["planar_sizing_direction_XYZ"]);
                options["facet_distance_mm"]/=unit;
                options["cell_fine_size_mm"]/=unit;
                options["cell_coarse_size_mm"]/=unit;
                options["cell_size_electrodes_mm"]/=unit;
                options["electrode_radius_mm"]/=unit;


        }
