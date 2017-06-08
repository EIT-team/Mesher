
#include "Sizing_fields.h"
#include "input_parameters.h"
#include "Matlab_save.h"
#include "save_dgf.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// include/defines used across all  statements
#include "include.h"

// To avoid verbose function and named parameters call
using namespace CGAL::parameters;
using namespace std;

#include <CGAL/config.h>

#include "from_matlab.h"

void printusage(void)
{
    printf("Usage:  -i input image file\n");
    printf("        -e electrode position file\n");
    printf("        -p parameter file\n");
    printf("        -o output file\n");
    exit(EXIT_FAILURE);
}



int main(int argc, char* argv[])
{

// Print CGAL Version number
std::cout << "CGAL Version " << CGAL_VERSION_NR << " (1MMmmb1000)" << std::endl;
std::cout << "where MM is the major number release, mm is the minor number release" << std::endl;

// Check input parameters

    if(argc < 9) printusage();
    int opt;
    char *path_image, *path_electrode, *path_parameter, *output_file;
    while((opt = getopt(argc, argv, "i:e:p:o:"))!=-1)
    {
        switch(opt)
        {
            case 'i':
                path_image = optarg;
            case 'e':
                path_electrode = optarg;
            case 'p':
                path_parameter = optarg;

            case 'o':
                output_file = optarg;

        }
    }
    if(optind != argc) printusage();

    // Output file names for sanity check
    std::cout << "Input file: " 		<< path_image << "\n";
    std::cout << "Electrode file: " 	<< path_electrode << "\n";
    std::cout << "Parameter file: " 	<< path_parameter << "\n";
    std::cout << "Output file: " 		<< path_image << "\n";

    // Loads image
    CGAL::Image_3 image;

    // Read input file with parameters
    Input p;
    p.load_file_idx(path_parameter);

    // Reading image file
    std::cout<<"\n Reading the Image file... ";
    image.read(path_image);


    // Domain
    Mesh_domain domain(image);

    //Define Sizing field
    // vx, vy and vz are the size of each voxel
   // xdim, ydim and zdim are the number of voxels along each axis
    Point origin(image.vx () * image.xdim ()/2,
            image.vy () * image.ydim ()/2, 
            image.vz () * image.zdim ()/2); //origin

    FILE *F;
    try { F=fopen(path_electrode,"r");}
    catch (exception& e) { cout << e.what() << endl;}

    Mesh_domain::Index sub = domain.index_from_subdomain_index(2); //!!! I do not remember what this does, but it should be very useful ...

    sizing_field_elliptic_electrodes sizing_field (origin,F,1/p.unit,1/p.unit,1/p.unit); //This is basic and working now for both rat and human

    if (F!=NULL) fclose(F);

    sizing_field.coarse_size=p.options["cell_coarse_size_mm"];
    sizing_field.fine_size=p.options["cell_fine_size_mm"];
    sizing_field.preserve=int(p.options["elements_with_fine_sizing_fieldercentage"]);
    sizing_field.e_R=2*p.options["electrode_radius_mm"]; //2* to secure fit of the electrode
    sizing_field.electrode_size=p.options["cell_size_electrodes_mm"];//Planar gradient with electrodes -- size of the mesh near electrodes


    // Mesh criteria: faces and cells
    Mesh_criteria criteria(facet_angle=p.options["facet_angle_deg"], facet_size=sizing_field, facet_distance=p.options["facet_distance_mm"],
            cell_radius_edge_ratio=p.options["cell_radius_edge_ratio"], cell_size=sizing_field);


    // Meshing
    std::cout<<"\n Meshing with initial mesh..." << endl;
    C3t3 c3t3;
    //try { //Do not need this anymore in x64 version, however x86 need one, slows down everything

    c3t3= CGAL::make_mesh_3<C3t3>(domain, criteria, CGAL::parameters::features(domain), 
            CGAL::parameters::no_lloyd(), CGAL::parameters::no_odt(), 
            CGAL::parameters::no_perturb(),CGAL::parameters::no_exude()); 
    //}

    //centre_of_mesh(c3t3,p);
    //Point test_closest(100,100,100);
    //test_closest_element(c3t3);
    cout << "Moving electrodes to closest facets: " << endl;    
    for(int i = 0; i < sizing_field.centres.size(); ++i) {
	sizing_field.centres[i] = closest_element(c3t3, sizing_field.centres[i]);
	}
    cout << "Finished moving electrodes" << endl;


    //Optimisation
    std::cout<<"\n Optimising: ";
    if (int(p.options["perturb_opt"])==1) {std::cout<<"\n Perturb... "; CGAL::perturb_mesh_3(c3t3, domain,sliver_bound=10, time_limit=p.options["time_limit_sec"]);}
    if (int(p.options["lloyd_opt"])==1)  {std::cout<<"\n Lloyd... ";CGAL::lloyd_optimize_mesh_3(c3t3, domain, time_limit=p.options["time_limit_sec"]);}
    if (int(p.options["odt_opt"])==1)  {std::cout<<"\n ODT... "; CGAL::odt_optimize_mesh_3(c3t3, domain, time_limit=p.options["time_limit_sec"]);}
    if (int(p.options["exude_opt"])==1)  {std::cout<<"\n Exude... "; CGAL::exude_mesh_3(c3t3, sliver_bound=10, time_limit=p.options["time_limit_sec"]);}

    // Output
	save_as_dgf(c3t3,p,output_file);
    //matlab output
    //std::cout<<"\n Saving the mesh into matlab file... ";
    //int save=save_matlab(c3t3,p,output_file);

    //all done
    std::cout<<"\n ALL DONE! :)" << endl;


    return 0;
}


