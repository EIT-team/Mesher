#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>

#include <CGAL/Labeled_image_mesh_domain_3.h>
#include <CGAL/Mesh_domain_with_polyline_features_3.h>

#include <CGAL/make_mesh_3.h>
#include <CGAL/Image_3.h>
//#include <File_medit.h>

#include <mat.h>

// Domain
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Labeled_image_mesh_domain_3<CGAL::Image_3,K> Mesh_domain;

//For inexact functions
typedef K::FT FT;
typedef K::Point_3 Point;
typedef K::Vector_3 Vector;

// Triangulation
typedef CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;

// Iterators for Facets, Vertices and cells
typedef C3t3::Facets_in_complex_iterator Facet_iterator;
typedef C3t3::Cells_in_complex_iterator Cell_iterator;

typedef Tr::Finite_vertices_iterator Finite_vertices_iterator;
typedef Tr::Vertex_handle Vertex_handle;

// Point
typedef Tr::Point Point_3;

// Map generators (connectivity and domains)
 typedef CGAL::Mesh_3::Medit_pmap_generator<C3t3,true,true> Generator;
 typedef  Generator::Cell_pmap Cell_pmap;
 typedef  Generator::Facet_pmap Facet_pmap;
 typedef  Generator::Facet_pmap_twice Facet_pmap_twice;
 typedef Generator::Vertex_pmap Vertex_pmap;
  
  
 // save triangulation in matlab Superstructure Mesh (Mesh.Nodes, Mesh.Tria, Mesh.Tetra)
int save_matlab (const C3t3& c3t3)
{

	// Doing some initial mapping
	Cell_pmap cell_pmap(c3t3);
	  Facet_pmap facet_pmap(c3t3,cell_pmap);
	  Facet_pmap_twice facet_pmap_twice(c3t3,cell_pmap);
	  Vertex_pmap vertex_pmap(c3t3,cell_pmap,facet_pmap);
	
	
	// Open Mat file 
	 MATFile *pmat;
  
    // Initialising triangulation
	  const Tr& tr = c3t3.triangulation();
//---------------------------------------------------
// Importing vertices into Nodes
//---------------------------------------------------
	  //This is the array which will be finally copied to Mat file, there is no iterators in the mat library, so the only possible way is to copy memory
	  double * Vtx =new double [tr.number_of_vertices()*4];
	  void * pVtx=Vtx; // this is points to the beginning
	  std::map<Vertex_handle, int> V; // This handles connectivity
	  int inum = 1; //this is for element access to V

	  // Copying vertices in the Vtx array
	  for( Finite_vertices_iterator vit = tr.finite_vertices_begin();
		   vit != tr.finite_vertices_end();
		   ++vit)
	  {
		V[vit] = inum++;
		Point p = vit->point();
		*Vtx= CGAL::to_double(p.x()); 
		*(Vtx+tr.number_of_vertices())= CGAL::to_double(p.y()); 
		*(Vtx+2*tr.number_of_vertices())= CGAL::to_double(p.z()); 
		*(Vtx+3*tr.number_of_vertices())=double(get(vertex_pmap, vit));
		Vtx++;
	  }
	  // Create the disgusting Matlab array and fill it with zeros, bastards Mathworks, let them handle type conversion till the end of their lifes!
	  mxArray * mVtx = mxCreateDoubleMatrix(tr.number_of_vertices(),4,mxREAL);
	  
	  // And here is the most painful structure here. NOTE: if you have troubles with it, i am not responsible, Matlab does! 
	  memcpy((void *)(mxGetPr(mVtx)), (void *)pVtx, int(tr.number_of_vertices()*4*sizeof(double)));
      
	  // Open the file and put the array in
	  pmat = matOpen("Mesh.mat", "w");
	  matPutVariable(pmat, "Nodes", mVtx);

	  // Cleanup
	  delete [] pVtx;
	  mxDestroyArray(mVtx);
////-------------------------------------------------------
//// TRIANGLES. This is a bit tricky because we actually need double triangles in the border of the domains.
////-------------------------------------------------------
	 
    C3t3::size_type number_of_triangles= c3t3.number_of_facets_in_complex();
	number_of_triangles+=number_of_triangles;

	// This is the array to be copied to Mat
	int * Fsc = new int [number_of_triangles*4];
	void * pFsc=Fsc;// remember begin, bastards don't have fucking iterators, remember!
     
	// Here is our thing, looks not bad to my knowladge
	for( Facet_iterator fit = c3t3.facets_in_complex_begin();
        fit != c3t3.facets_in_complex_end();
         ++fit)
	   {
		 int t=0;
		 for (int i=0; i<4; i++)
		 {
		  if (i != fit->second)
		  {
			const Vertex_handle& vh = (*fit).first->vertex(i);
			*(Fsc+t*number_of_triangles) = int(V[vh]);
			t++;
		  }
		}
	    *(Fsc+t*number_of_triangles)= int(get(facet_pmap, *fit));
		Fsc++;

		t=0;
	   for (int i=0; i<4; i++)
       {
        if (i != fit->second)
        {
			const Vertex_handle& vh = (*fit).first->vertex(i);
			*(Fsc+t*number_of_triangles) = int(V[vh]);
			t++;
		}
	   }
		 *(Fsc+t*number_of_triangles)= int(get(facet_pmap_twice, *fit));
		Fsc++;
	   
   }
	
	// Create the disgusting Matlab array and fill it with zeros, bastards Mathworks, let them handle type conversion till the end of their lifes!
	mxArray * mFsc = mxCreateNumericMatrix(number_of_triangles,4,mxINT32_CLASS,mxREAL);
	
	// And here is the most painful structure here. NOTE: if you have troubles with it, i am not responsible, Matlab does! 
	memcpy((void *)(mxGetPr(mFsc)), (void *)pFsc, int(number_of_triangles*4*sizeof(int)));
	
	// Put in Mat and cleanup
	matPutVariable(pmat, "Tria", mFsc);
	delete [] pFsc;
	mxDestroyArray(mFsc);
	
  ////-------------------------------------------------------
  //// Tetrahedra
  ////-------------------------------------------------------
   
	C3t3::size_type number_of_cells = c3t3.number_of_cells_in_complex();
	
	// Array to put in mat
	int * Ttr = new int [number_of_cells*5];
	void * pTtr=Ttr; // remember the beginning 

   // Copy cells in the Ttr
	for( Cell_iterator cit = c3t3.cells_in_complex_begin() ;
      cit != c3t3.cells_in_complex_end() ;
      ++cit )
   {
	for (int i=0; i<4; i++)
			{ *(Ttr+i*number_of_cells) = int( V[cit->vertex(i)]) ;}
    
    *(Ttr+4*number_of_cells) = int(get(cell_pmap, cit));
	Ttr++;
   }
   
   // Create the disgusting Matlab array and fill it with zeros, bastards Mathworks, let them handle type conversion till the end of their lifes!
   mxArray * mTtr = mxCreateNumericMatrix(number_of_cells,5,mxINT32_CLASS,mxREAL);

   // And here is the most painful structure here. NOTE: if you have troubles with it, i am not responsible, Matlab does! 
   memcpy((void *)(mxGetPr(mTtr)), (void *)pTtr, int (number_of_cells*5*sizeof(int)));
   
   // Put the stuff in mat and cleanup
   matPutVariable(pmat, "Tetra", mTtr);
	delete [] pTtr;
	mxDestroyArray(mTtr);

	
  ////-------------------------------------------------------
  //// End
  ////-------------------------------------------------------
   
	// cloas the file and return if we actually closed it. If it is not closed it is impossible to open afterwards in Matlab 
	if (matClose(pmat)) return 1;
   return 0;

	
}