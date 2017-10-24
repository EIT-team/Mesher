#include "CGAL_include.h"

using namespace std;

Point centre_of_mesh(const C3t3& c3t3);

//TODO: move definition to cpp file, need to look at CMAKE to get it to include the cpp file
Point centre_of_mesh(const C3t3& c3t3) {

								cout << "Calculating cetnre of mesh" << endl;

								const Tr& tr = c3t3.triangulation();
								double n_vertex = tr.number_of_vertices();
								double mean_vtx[3] = {0};

								map<Vertex_handle, int> V;
								int inum = 1;
								Point p;

								for (Finite_vertices_iterator vit = tr.finite_vertices_begin(); vit != tr.finite_vertices_end(); ++vit)
								{
																p = vit->point();
																mean_vtx[0] += CGAL::to_double(p.x());
																mean_vtx[1] += CGAL::to_double(p.y());
																mean_vtx[2] += CGAL::to_double(p.z());

								}

								Point centre_point(mean_vtx[0]/n_vertex, mean_vtx[1]/n_vertex, mean_vtx[2]/n_vertex);

								cout << "x: " << centre_point.x() << endl
																		<< "y: " << centre_point.y() << endl
																		<< "z: " << centre_point.z() << endl;


								return centre_point;
}

Point closest_element(const C3t3& c3t3, Point target_p) {
	// Find the closest element in the surface to a given point in space

								// Facet property map - used for finding the domain (tissue type) of a facet or cell
								Cell_pmap cell_pmap(c3t3);
								Facet_pmap facet_pmap(c3t3,cell_pmap);

								//cout << "Finding closest surface element to " << target_p << endl;
								//cout << "Number of facets in complex: " << c3t3.number_of_facets_in_complex() << endl
								;
								double min_dist = 1000000; //Large sentinel value
								double this_dist;
								Point centre_of_closest(0,0,0);
								vector<Point> facet_points;

								// Only want to use facets that are in the outermost layer i.e. the skin
								//TODO: Don't hardcode tihs value?
								int skin_tissue_index = 7;


								// Iterate over all facets
								for (Facet_iterator facet_iterator = c3t3.facets_in_complex_begin(); facet_iterator != c3t3.facets_in_complex_end(); ++facet_iterator) {

																// Get domain number/tissue type using facet map
																int this_tissue = int(get(facet_pmap, *facet_iterator));

																// Check facet is the tissue type we want
																if ( this_tissue == skin_tissue_index) {
																								for (int i = 0; i < 4; ++i) {

																																// We only want to store the three verticies of the cell that correspond to this facet
																																// ->second gives the index of the vertex opposite to the facet, which is the one we don't want
																																if ( i != facet_iterator->second) {
																																								facet_points.push_back( facet_iterator->first->vertex(i)->point() );
																																}
																								}

																								//Calcualte centre of the facet
																								Point centre_of_facet = CGAL::centroid(facet_points[0], facet_points[1], facet_points[2]);
																								//cout << "Centre: " << centre_of_facet <<endl;

																								// how far is this facet from the target point
																								this_dist = CGAL::squared_distance(target_p, centre_of_facet);

																								// If closer than current minimum, store it and update min distance
																								if (this_dist < min_dist) {
																																min_dist = this_dist;
																																centre_of_closest = centre_of_facet;

																																//cout << " Centre: " <<      centre_of_facet << " Distance: " << this_dist <<endl;
																																//cout << "Facet type: " << get(facet_pmap, *facet_iterator) << endl;
																								}

																								// Empty facet_points, otherwise the [0] [1] [2] indexing used for CGAL::centroid won't do anything with new data
																								facet_points.clear();

																}
								}

								cout << "Closest point to: " << target_p << " is: " << centre_of_closest << endl;
								return centre_of_closest;

}

bool test_closest_element(const C3t3& c3t3) {

	// TODO: This only works for a  particualr mesh (can't remember which)
								cout << "Testing calculation of closest element" << endl;

								Point test_closest(100,100,100);
								Point result = closest_element(c3t3, test_closest);
								Point expected(60.1738, 78.1268, 105.536);

								double max_distance = 0.1; // How far apart can the result be from expected value
								double actual_distance = CGAL::squared_distance(result,expected);

								// Check if the expected value is sufficiently close to the result
								if (actual_distance > max_distance) {
																cout << "Test failed!" << endl;
																return false;
								}

								cout << "Test passed!" << endl;

								return true;
}


vector<Point> load_electrode_locations(FILE *F, FT scale) {
								// Loads electrode positions from a file and returns a vector of points
								vector<Point> electrode_locations;

								if (F == NULL) perror ("Error opening electrode file");
								else {
																while(!feof(F))
																{
																								float x,y,z;
																								fscanf(F,"%f,%f,%f\n",&x,&y,&z);
																								Point pt(x*scale,y*scale,z*scale);
																								electrode_locations.push_back(pt);
																}
								}

								return electrode_locations;

}

Point set_reference_electrode(const C3t3& c3t3)
{
								// Add a sufficently large vector to the centre of the mesh,
								// should result in a point outside of the mesh.
								// This particular vector should come out of the front of the forehead
								// for human head

								// TODO: Combine this with Deform_Volume class to allow accessing of mesh_bounds?
								// Then won't need to guestimate a point outside of the mesh.
								cout << "Generating reference electrode location\n";

								Vector far_away(0,-150,50); 	//Extend from centre of forehead
								//Point centre = centre_of_mesh(c3t3);
								//double zmax = c3t3.zdim() * c3t3.vz();
								//Point outside_mesh( CGAL::to_double(centre.x()),0,zmax);

								Point outside_mesh = centre_of_mesh(c3t3) + far_away;
								Point reference = closest_element( c3t3, outside_mesh);

								cout << "Point outside mesh is: " << outside_mesh << endl;
								cout << "Reference located at: " << reference << endl << endl;

								return reference;
}

Point set_ground_electrode(const C3t3& c3t3)
{
								// Place the ground location at the back of the head (For humans)

								cout << "Placing ground elecrtode\n";


								const Tr& tr = c3t3.triangulation();
								double n_vertex = tr.number_of_vertices();
								Point gnd_electrode, current_vertex;
								double furthest = -1000; // Set to sentinel values
								double current_y;

//map<Vertex_handle, int> V;
								int inum = 1;
//Point p;

// Find the largest y value by iterating through all vertices
								for (Finite_vertices_iterator vit = tr.finite_vertices_begin(); vit != tr.finite_vertices_end(); ++vit)
								{

																current_vertex = vit->point();
																current_y = CGAL::to_double(current_vertex.y());

																// Check if current value is the largest and updtate if so
																if (current_y > furthest) {
																								furthest = current_y;
																								gnd_electrode = current_vertex;
																}

								}

								cout << "Ground electrode placed at: " << gnd_electrode << endl << endl;
								return gnd_electrode;

}

vector<int> generate_full_protocol(vector<int> prt, int n_elecs)
{
								// Take input protocol as a vector of ints.
								// Each pair of ints represents one injection pair.
								// e.g.  [2 5 3 7] means injection between 2,5 and injeciton between 3,7
								cout << "generating full protocol\n";
								int n_prt = prt.size()/2;

								int gnd_elec = n_elecs + 1;
								int injection_a, injection_b;

								vector<int> full_prt;

								for (int i = 0; i < n_prt; i++)
								{
																injection_a = prt[2*i];
																injection_b = prt[2*i +1];

																cout << "Injection electrodes: " << injection_a << " " << injection_b << endl;

																for (int elec_num = 1; elec_num <= n_elecs; elec_num++)
																{
																								// Don't include injection electrodes in measurements
																								if (elec_num != injection_a && elec_num != injection_b)
																								{
																																// Add prt row - inj_a inj_b meas gnd
																																full_prt.push_back(gnd_elec);
																																full_prt.push_back(elec_num);
																																full_prt.push_back(injection_b);
																																full_prt.push_back(injection_a);

																								}
																}
								}
								return full_prt;
}
