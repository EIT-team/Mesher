// Saves the structure in dgf format
// Refer to Matlab documentation for mat.h for help
// Thomas Dowrick

#include "include.h"
// Write Mesh as DGF file, to replace matlab dune_exporter function

//TODO: Write electrode and dune parameter file
//TODO: Add ground/reference electrode locaion
void save_as_dgf (const C3t3& c3t3, Input st, std::string output_file)
{

								std::cout << "Writing dgf file: " << output_file << '\n';

								//! Doing some initial mapping
								Cell_pmap cell_pmap(c3t3);
								Facet_pmap facet_pmap(c3t3,cell_pmap);
								Facet_pmap_twice facet_pmap_twice(c3t3,cell_pmap);
								Vertex_pmap vertex_pmap(c3t3,cell_pmap,facet_pmap);

								std::map<Vertex_handle, int> vertex_map; // This handles connectivity of vertex_handles to vertex number

								int n_node = 1;
								int n_tetra = 1;

								Point current_point;
								double x,y,z;
								int cell_nodes[4];
								double cell_domain; // What domain/tissue type is the cell

								const Tr& triangulation = c3t3.triangulation();

								Finite_vertices_iterator vertices_iterator;
								Cell_iterator cell_iterator;

								FILE *dgf_file;
								dgf_file = fopen(output_file.c_str(), "w"); // Convert str to char* for fopen command

								// Write Nodes
								fprintf(dgf_file, "DGF\n");
								fprintf(dgf_file, "vertex\n");
								fprintf(dgf_file, "firstindex 1\n");

								for (vertices_iterator = triangulation.finite_vertices_begin(); vertices_iterator != triangulation.finite_vertices_end(); ++vertices_iterator) {

																// Store vertex info in vertex_handle to vertex map
																vertex_map[vertices_iterator] = n_tetra++;

																current_point = vertices_iterator->point();

																x = CGAL::to_double(current_point.x()*st.unit);
																y = CGAL::to_double(current_point.y()*st.unit);
																z = CGAL::to_double(current_point.z()*st.unit);

																fprintf(dgf_file, "%6.18f %6.18f %6.18f # %d\n", x, y, z, n_node++);
								}

								fprintf(dgf_file, "#\n");

								// Write Tetrahedron
								fprintf(dgf_file, "Simplex\n");
								fprintf(dgf_file, "parameters 2\n");

								n_tetra = 1; //reset

								for (cell_iterator = c3t3.cells_in_complex_begin(); cell_iterator != c3t3.cells_in_complex_end(); ++cell_iterator) {

																for (int i = 0; i < 4; i++) {
																								// We want to store the number of the vertex, as stored in 'Nodes' rather than the vertex cordinates itself
																								cell_nodes[i] = int(vertex_map[ cell_iterator->vertex(i)]);
																}

																// get the cell domain/tissue index
																cell_domain = double(get(cell_pmap, cell_iterator));

																fprintf(dgf_file, "%d %d %d %d %f %d\n", cell_nodes[0], cell_nodes[1], cell_nodes[2], cell_nodes[3], cell_domain, n_tetra++);
								}

								fprintf(dgf_file, "#\n");

								fclose(dgf_file);
								std::cout << "Finished writing" << '\n';
}


void save_electrodes(Points electrodes, std::string output_file)
{
							std::cout << "Writing electrode file: " << output_file << '\n';

								FILE *electrode_file;
								electrode_file  =fopen(output_file.c_str(), "w");

								for (int i = 0; i < electrodes.size(); i++) {
																fprintf(electrode_file, "%6.18f %6.18f %6.18f \n", electrodes[i].x(), electrodes[i].y(), electrodes[i].z());
								}

								fclose(electrode_file);
								std::cout << "Finished writing" << '\n';

}

void save_parameters(const C3t3& c3t3, Input st, std::string output_file)
{


}
