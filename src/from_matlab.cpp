#include "include.h"

void centre_of_mesh(const C3t3& c3t3, Input st) {

	cout << "Calculating cetnre of mesh" << endl;

	const Tr& tr = c3t3.triangulation();
	double *Vtx = new double[tr.number_of_vertices()*4];
	double mean_vtx[3] = {0};
	void *pVtx = Vtx; //point to beginning
	std::map<Vertex_handle, int> V;
	int inum = 1;
	Point p;

	for (Finite_vertices_iterator vit = tr.finite_vertices_begin(); vit != tr.finite_vertices_end(); ++vit)
	{
		p = vit->point();
		mean_vtx[0] += CGAL::to_double(p.x());
		mean_vtx[1] += CGAL::to_double(p.y());
		mean_vtx[2] += CGAL::to_double(p.z());
		
	}

	std::cout << "x: " << mean_vtx[0] << endl;

	

}
