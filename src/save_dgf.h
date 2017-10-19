// Saves the structure in dgf format
// Refer to Matlab documentation for mat.h for help
// Thomas Dowrick

#include "CGAL_include.h"

using namespace std;

// Write Mesh as DGF file, to replace matlab dune_exporter function

//TODO: Write electrode and dune parameter file
//TODO: Add ground/reference electrode locaion
void save_as_dgf (const C3t3& c3t3, Input st, string output_file)
{
								output_file += ".dgf";
								cout << "Writing dgf file: " << output_file << '\n';

								//! Doing some initial mapping
								Cell_pmap cell_pmap(c3t3);
								Facet_pmap facet_pmap(c3t3,cell_pmap);
								Facet_pmap_twice facet_pmap_twice(c3t3,cell_pmap);
								Vertex_pmap vertex_pmap(c3t3,cell_pmap,facet_pmap);

								map<Vertex_handle, int> vertex_map; // This handles connectivity of vertex_handles to vertex number

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
								cout << "Finished writing" << "\n\n";
}


void save_electrodes(Points electrodes, string output_file)
{
								output_file += "_electrodes.txt";
								cout << "Writing electrode file: " << output_file << '\n';

								FILE *electrode_file;
								electrode_file  =fopen(output_file.c_str(), "w");

								for (int i = 0; i < electrodes.size(); i++) {
																fprintf(electrode_file, "%6.18f, %6.18f, %6.18f \n", electrodes[i].x(), electrodes[i].y(), electrodes[i].z());
								}

								fclose(electrode_file);
								cout << "Finished writing" << "\n\n";

}


void save_parameters(map<string, string> parameters, string output_file)
{
								output_file += "_parameters";
								cout << "Writing parameter file: " << output_file << '\n';

								// Use stream for writing output file, easier to use with map data structure that fprintf
								fstream parameter_file;
								parameter_file.open(output_file.c_str(), fstream::out);

								map<string, string>::iterator it;

								for (it = parameters.begin(); it != parameters.end(); it++)
								{
																parameter_file << it->first << ": " << it->second << endl << endl;
								}

								parameter_file.close();
								cout << "Finished writing \n\n";

}


void save_protocol(vector<int> full_prt, string output_file)
{
	// Take output protocol as a vector of ints.
	// Each sequence of four ints represents one protocol line
	// e.g.  	inj_a inj_b meas gnd

	output_file += "_protocol";
	cout << "writing full protocol to file: " << output_file << endl;
	int n_prt = full_prt.size()/4;

		FILE *protocol_file;
		protocol_file = fopen(output_file.c_str(), "w");

		int i; // Index of current vector element
		for (int j = 0; j < n_prt; j++)
		{
			//Write protocol line - next four elements of vector
			fprintf(protocol_file, "%d %d %d %d\n", full_prt[i++], full_prt[i++], full_prt[i++], full_prt[i++]);
		}

		fclose(protocol_file);

		cout << "Finished writing\n";
}

void write_centres(C3t3& c3t3, string output_file) {
	// Calculate the centres of each cell and write to a file

	output_file += "_cell_centres";
	cout << "Writing cell centres to file: " << output_file << endl;

	Point p;
	int num_vertices = 4;
	int tissue_type;
	FILE *centres_file;
	centres_file = fopen(output_file.c_str(), "w");

	Cell_iterator cell_iterator;

	for (cell_iterator = c3t3.cells_in_complex_begin(); cell_iterator != c3t3.cells_in_complex_end(); ++cell_iterator) {

		// reset centre
		double centre[3] = {0};

		tissue_type = (int)(c3t3.subdomain_index(cell_iterator));

		for (int i = 0; i < num_vertices; i++) {
				p = cell_iterator->vertex(i)->point();

				centre[0] += CGAL::to_double(p.x());
				centre[1] += CGAL::to_double(p.y());
				centre[2] += CGAL::to_double(p.z());

			}

			// Calculate centre of cell by taking average
			centre[0] /= num_vertices;
			centre[1] /= num_vertices;
			centre[2] /= num_vertices;

			// Write centres and subdomain/tissue type
			fprintf(centres_file, "%6.5f %6.5f %6.5f %d\n", centre[0], centre[1], centre[2], tissue_type);

		}

	fclose(centres_file);
	cout << "Finished centres" << endl;
}
