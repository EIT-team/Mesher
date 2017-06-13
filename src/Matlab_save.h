// Saves the structure in the matlab format
// Refer to Matlab documentation for mat.h for help
// developed by Kirill Aristovich

#include <mat.h>
#include "include.h"

//! save triangulation in matlab Superstructure Mesh (Mesh.Nodes, Mesh.Tria, Mesh.Tetra)
int save_matlab (const C3t3& c3t3, Input st, char* output_file)
{

        //! Doing some initial mapping
        Cell_pmap cell_pmap(c3t3);
        Facet_pmap facet_pmap(c3t3,cell_pmap);
        Facet_pmap_twice facet_pmap_twice(c3t3,cell_pmap);
        Vertex_pmap vertex_pmap(c3t3,cell_pmap,facet_pmap);


        //! Open Mat file
        MATFile *pmat;

        //! Initialising triangulation
        const Tr& tr = c3t3.triangulation();
        //!---------------------------------------------------
        //! Importing vertices into Nodes
        //!---------------------------------------------------
        //! This is the array which will be finally copied to Mat file, there is
        //! no iterators in the mat library, so the only possible way is to copy memory
        double * Vtx =new double [tr.number_of_vertices()*4];
        void * pVtx=Vtx; //! this is points to the beginning
        std::map<Vertex_handle, int> V; //! This handles connectivity
        int inum = 1; //! this is for element access to V

        //! Copying vertices in the Vtx array
        for( Finite_vertices_iterator vit = tr.finite_vertices_begin();
             vit != tr.finite_vertices_end();
             ++vit)
        {
                V[vit] = inum++;
                Point p = vit->point();
                *Vtx= CGAL::to_double(p.x()*st.unit);
                *(Vtx+tr.number_of_vertices())= CGAL::to_double(p.y()*st.unit);
                *(Vtx+2*tr.number_of_vertices())= CGAL::to_double(p.z()*st.unit);
                *(Vtx+3*tr.number_of_vertices())=double(get(vertex_pmap, vit));
                Vtx++;
        }
        //! Create the disgusting Matlab array and fill it with zeros,
        //! bastards Mathworks, let them handle type conversion till the end of their lifes!
        mxArray * mVtx = mxCreateDoubleMatrix(tr.number_of_vertices(),4,mxREAL);

        //! And here is the most painful structure here. NOTE: if you have troubles with it, i am not responsible, Matlab does!
        memcpy((void *)(mxGetPr(mVtx)), (void *)pVtx, int(tr.number_of_vertices()*4*sizeof(double)));

        //! Open the file and put the array in
        pmat = matOpen(output_file, "w");
        matPutVariable(pmat, "Nodes", mVtx);

        //! Cleanup
        delete [] pVtx;
        mxDestroyArray(mVtx);
        //!-------------------------------------------------------
        //! TRIANGLES. This is a bit tricky because we actually need double triangles in the border of the domains.
        //!-------------------------------------------------------

        C3t3::size_type number_of_triangles= c3t3.number_of_facets_in_complex();
        number_of_triangles+=number_of_triangles;

        //! This is the array to be copied to Mat
        int * Fsc = new int [number_of_triangles*4];
        void * pFsc=Fsc;//! remember begin, bastards don't have fucking iterators, remember!

        //! Here is our thing, looks not bad to my knowladge
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

        //! Create the disgusting Matlab array and fill it with zeros, bastards Mathworks,
        //! let them handle type conversion till the end of their lifes!
        mxArray * mFsc = mxCreateNumericMatrix(number_of_triangles,4,mxINT32_CLASS,mxREAL);

        //! And here is the most painful structure here. NOTE: if you have troubles with it, i am not responsible, Matlab does!
        memcpy((void *)(mxGetPr(mFsc)), (void *)pFsc, int(number_of_triangles*4*sizeof(int)));

        //! Put in Mat and cleanup
        matPutVariable(pmat, "Tria", mFsc);
        delete [] pFsc;
        mxDestroyArray(mFsc);

        //!-------------------------------------------------------
        //! Tetrahedra
        //!-------------------------------------------------------

        C3t3::size_type number_of_cells = c3t3.number_of_cells_in_complex();

        //! Array to put in mat
        int * Ttr = new int [number_of_cells*5];
        void * pTtr=Ttr; // remember the beginning

        //! Copy cells in the Ttr
        for( Cell_iterator cit = c3t3.cells_in_complex_begin();
             cit != c3t3.cells_in_complex_end();
             ++cit )
        {
                for (int i=0; i<4; i++)
                { *(Ttr+i*number_of_cells) = int( V[cit->vertex(i)]);}

                *(Ttr+4*number_of_cells) = int(get(cell_pmap, cit));
                Ttr++;
        }

        //! Create the disgusting Matlab array and fill it with zeros, bastards Mathworks,
        //! let them handle type conversion till the end of their lifes!
        mxArray * mTtr = mxCreateNumericMatrix(number_of_cells,5,mxINT32_CLASS,mxREAL);

        //! And here is the most painful structure here. NOTE: if you have troubles with it, i am not responsible, Matlab does!
        memcpy((void *)(mxGetPr(mTtr)), (void *)pTtr, int (number_of_cells*5*sizeof(int)));

        //! Put the stuff in mat and cleanup
        matPutVariable(pmat, "Tetra", mTtr);
        delete [] pTtr;
        mxDestroyArray(mTtr);


        ////-------------------------------------------------------
        //// End
        ////-------------------------------------------------------

        //! cloas the file and return if we actually closed it. If it is not closed it is impossible to open afterwards in Matlab
        if (matClose(pmat)) return 1;
        return 0;


}
