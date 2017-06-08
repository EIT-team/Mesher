void centre_of_mesh(const C3t3& c3t3, Input st);

//TODO: move definition to cpp file, need to look at CMAKE to get it to include the cpp file
void centre_of_mesh(const C3t3& c3t3, Input st) {

	cout << "Calculating cetnre of mesh" << endl;

	const Tr& tr = c3t3.triangulation();
	double n_vertex = tr.number_of_vertices();
	double mean_vtx[3] = {0};
	
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

	std::cout 	<< "x: " << mean_vtx[0]/n_vertex << endl
			<< "y: " << mean_vtx[1]/n_vertex << endl
			<< "z: " << mean_vtx[2]/n_vertex << endl;

	

}

//TODO: Iterate over all electrodes
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
	std::vector<Point> facet_points;
	
	// Only want to use facets that are in the outermost layer i.e. the skin
	int skin_tissue_index = 7;

	
	// Iterate over all facets
	for (Facet_iterator facet_iterator = c3t3.facets_in_complex_begin(); facet_iterator != c3t3.facets_in_complex_end(); ++facet_iterator) {

		// Get domain number/tissue type using facet map
		int this_tissue = int(get(facet_pmap, *facet_iterator));

		// Check facet is the tissue type we want
		if ( this_tissue == skin_tissue_index) {
			for (int i = 0; i < 4 ; ++i) {
			
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
			
				//cout << " Centre: " << 					centre_of_facet << " Distance: " << this_dist <<endl;
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


std::vector<Point> load_electrode_locations(FILE *F, FT scale) {
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

