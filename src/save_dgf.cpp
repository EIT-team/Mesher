// Saves the structure in dgf format
// Refer to Matlab documentation for mat.h for help
// Thomas Dowrick
#include "save_dgf.h"

using namespace std;

/** Write Mesh as Dune Grid Format file.
 *  Inputs: mesh, vector of parementers, and output file name.
 **/
void save_as_dgf(const C3t3 &c3t3, std::map<std::string, FT> options, string output_file)
{
	output_file += ".dgf";
	cout << "Writing dgf file: " << output_file << '\n';

	// Doing some initial mapping
	Cell_pmap cell_pmap(c3t3);
	Facet_pmap facet_pmap(c3t3, cell_pmap);
	Facet_pmap_twice facet_pmap_twice(c3t3, cell_pmap);
	Vertex_pmap vertex_pmap(c3t3, cell_pmap, facet_pmap);

	map<Vertex_handle, int> vertex_map; // This handles connectivity of vertex_handles to vertex number

	int n_node = 1;
	int n_tetra = 1;

	Point current_point;
	double x, y, z;
	int cell_nodes[4];
	double cell_domain; // What domain/tissue type is the cell

	const Tr &triangulation = c3t3.triangulation();

	Finite_vertices_iterator vertices_iterator;
	Cell_iterator cell_iterator;

	FILE *dgf_file;
	dgf_file = fopen(output_file.c_str(), "w"); // Convert str to char* for fopen command

	// Write Nodes
	fprintf(dgf_file, "DGF\n");
	fprintf(dgf_file, "vertex\n");
	fprintf(dgf_file, "firstindex 1\n");

	for (vertices_iterator = triangulation.finite_vertices_begin(); vertices_iterator != triangulation.finite_vertices_end(); ++vertices_iterator)
	{

		// Store vertex info in vertex_handle to vertex map
		vertex_map[vertices_iterator] = n_tetra++;

		current_point = Point(vertices_iterator->point());

		// Convert x,y,z to metres before writing
		x = CGAL::to_double(current_point.x()) / MM_TO_M;
		y = CGAL::to_double(current_point.y()) / MM_TO_M;
		z = CGAL::to_double(current_point.z()) / MM_TO_M;

		fprintf(dgf_file, "%6.18f %6.18f %6.18f # %d\n", x, y, z, n_node++);
	}

	fprintf(dgf_file, "#\n");

	// Write Tetrahedron
	fprintf(dgf_file, "Simplex\n");
	fprintf(dgf_file, "parameters 2\n");

	n_tetra = 1; //reset

	for (cell_iterator = c3t3.cells_in_complex_begin(); cell_iterator != c3t3.cells_in_complex_end(); ++cell_iterator)
	{

		for (int i = 0; i < 4; i++)
		{
			// We want to store the number of the vertex, as stored in 'Nodes' rather than the vertex cordinates itself
			cell_nodes[i] = int(vertex_map[cell_iterator->vertex(i)]);
		}

		// get the cell domain/tissue index
		cell_domain = double(get(cell_pmap, cell_iterator));

		fprintf(dgf_file, "%d %d %d %d %f %d\n", cell_nodes[0], cell_nodes[1], cell_nodes[2], cell_nodes[3], cell_domain, n_tetra++);
	}

	fprintf(dgf_file, "#\n");

	fclose(dgf_file);
}

/** Write electrode positons to file.
 *  Inputs: electrode positions, output file name.
 **/
void save_electrodes(Points electrodes, string output_file)
{
	output_file += ".electrodes";
	cout << "Writing electrode file: " << output_file << '\n';

	FILE *electrode_file;
	electrode_file = fopen(output_file.c_str(), "w");

	for (int i = 0; i < electrodes.size(); i++)
	{
		fprintf(electrode_file, "%6.18f, %6.18f, %6.18f \n",
				electrodes[i].x() / MM_TO_M, electrodes[i].y() / MM_TO_M, electrodes[i].z() / MM_TO_M);
	}

	fclose(electrode_file);
}

/** Write parameters to file.
 *  Inputs: map of parameters, output file name.
 **/
void save_parameters(map<string, string> parameters, string output_file)
{
	output_file += ".parameters";
	cout << "Writing parameter file: " << output_file << '\n';

	// Use stream for writing output file, easier to use with map data structure that fprintf
	ofstream parameter_file;
	parameter_file.open(output_file.c_str());

	if (parameter_file.is_open())
	{
		parameter_file << "# Mesher parameters" << endl;

		for (auto param_elem : parameters)
		{
			//cout << elem.first << ": " << elem.second << endl; // print the parameters and the values
			parameter_file << param_elem.first << ": " << param_elem.second << endl; // print the parameters and the values
		}

		parameter_file.close();
	}
	else
	{
		cout << "Unable to open parameter file";
	}
}

/** Calcualte the centres of each cell and write to a file.
 * Inputs: Mesh, output file name.
 **/
void write_centres(C3t3 &c3t3, string output_file)
{

	output_file += ".cell_centres";
	cout << "Writing cell centres to file: " << output_file << endl;

	Point p;
	int num_vertices = 4;
	int tissue_type;
	FILE *centres_file;
	centres_file = fopen(output_file.c_str(), "w");

	Cell_iterator cell_iterator;

	for (cell_iterator = c3t3.cells_in_complex_begin(); cell_iterator != c3t3.cells_in_complex_end(); ++cell_iterator)
	{

		// reset centre
		double centre[3] = {0};

		tissue_type = (int)(c3t3.subdomain_index(cell_iterator));

		for (int i = 0; i < num_vertices; i++)
		{
			p = Point(cell_iterator->vertex(i)->point());

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
}

/** Write Mesh so that it can be opened in Matlab
 *  Inputs: mesh, vector of parementers, and output file name.
 **/
void save_matlab(const C3t3 &c3t3, std::map<std::string, FT> options, string output_file)

{

	string output_file_v = output_file;
	string output_file_t = output_file;

	output_file_v += "_vertices.csv";
	cout << "Writing vertices file: " << output_file_v << '\n';

	//! Doing some initial mapping
	Cell_pmap cell_pmap(c3t3);
	Facet_pmap facet_pmap(c3t3, cell_pmap);
	Facet_pmap_twice facet_pmap_twice(c3t3, cell_pmap);
	Vertex_pmap vertex_pmap(c3t3, cell_pmap, facet_pmap);

	map<Vertex_handle, int> vertex_map; // This handles connectivity of vertex_handles to vertex number

	int n_node = 1;
	int n_tetra = 1;

	Point current_point;
	double x, y, z;
	int cell_nodes[4];
	double cell_domain; // What domain/tissue type is the cell

	const Tr &triangulation = c3t3.triangulation();

	Finite_vertices_iterator vertices_iterator;
	Cell_iterator cell_iterator;

	FILE *vert_file;
	vert_file = fopen(output_file_v.c_str(), "w"); // Convert str to char* for fopen command

	for (vertices_iterator = triangulation.finite_vertices_begin(); vertices_iterator != triangulation.finite_vertices_end(); ++vertices_iterator)
	{

		// Store vertex info in vertex_handle to vertex map
		vertex_map[vertices_iterator] = n_tetra++;

		current_point = Point(vertices_iterator->point());

		// Convert x,y,z to metres before writing
		x = CGAL::to_double(current_point.x()) / MM_TO_M;
		y = CGAL::to_double(current_point.y()) / MM_TO_M;
		z = CGAL::to_double(current_point.z()) / MM_TO_M;

		fprintf(vert_file, "%6.18f, %6.18f, %6.18f \n", x, y, z);
	}

	fclose(vert_file);

	output_file_t += "_tetra.csv";
	cout << "Writing tetra file: " << output_file_t << '\n';

	FILE *tetra_file;
	tetra_file = fopen(output_file_t.c_str(), "w"); // Convert str to char* for fopen command

	n_tetra = 1; //reset

	for (cell_iterator = c3t3.cells_in_complex_begin(); cell_iterator != c3t3.cells_in_complex_end(); ++cell_iterator)
	{

		for (int i = 0; i < 4; i++)
		{
			// We want to store the number of the vertex, as stored in 'Nodes' rather than the vertex cordinates itself
			cell_nodes[i] = int(vertex_map[cell_iterator->vertex(i)]);
		}

		// get the cell domain/tissue index
		cell_domain = double(get(cell_pmap, cell_iterator));

		fprintf(tetra_file, "%d, %d, %d, %d, %f\n", cell_nodes[0], cell_nodes[1], cell_nodes[2], cell_nodes[3], cell_domain);
	}

	fclose(tetra_file);
}
