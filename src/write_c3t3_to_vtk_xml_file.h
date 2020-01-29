//
//  write_c3t3_to_vtk_xml_file.h
//
//  Created by David Bernstein on 4/23/13.
//

#ifndef Mesh_3_example_write_c3t3_to_vtk_xml_file_h
#define Mesh_3_example_write_c3t3_to_vtk_xml_file_h

/** Write mesh as a vtu file.
 * Inputs: Mesh, output file name.
 * Return True if succesful, false otherwise.
 **/
namespace CGAL
{
template <class C3t3>
bool write_c3t3_to_vtk_xml_file(const C3t3 &c3t3, const std::string &file_name)
{
    typedef typename C3t3::Triangulation Tr;
    typedef typename C3t3::Cells_in_complex_iterator Cell_iterator;
    typedef typename Tr::Finite_vertices_iterator Vertex_iterator;

    // Domain
    typedef Exact_predicates_inexact_constructions_kernel K;
    typedef K::FT FT;
    typedef K::Point_3 Point;

    // check that file extension is "vtu"
    CGAL_assertion(file_name.substr(file_name.length() - 4, 4) == ".vtu");

    // open file
    std::ofstream vtk_file(file_name.c_str());

    // header
    vtk_file << "<VTKFile type=\"UnstructuredGrid\" ";
    vtk_file << "version=\"0.1\" ";
    vtk_file << "byte_order=\"BigEndian\">" << std::endl;

    int indent_size = 2;
    std::string indent_unit(indent_size, ' ');
    std::string indent = indent_unit;
    vtk_file << indent + "<UnstructuredGrid>" << std::endl;

    // write mesh
    Tr t = c3t3.triangulation();
    int num_vertices = t.number_of_vertices();
    int num_cells = c3t3.number_of_cells_in_complex();

    indent += indent_unit;
    vtk_file << indent + "<Piece NumberOfPoints=\"" << num_vertices << "\" ";
    vtk_file << "NumberOfCells=\"" << num_cells << "\">" << std::endl;

    // Write vertices
    indent += indent_unit;
    vtk_file << indent + "<Points>" << std::endl;

    indent += indent_unit;
    vtk_file << indent;
    vtk_file << "<DataArray type=\"Float32\" NumberOfComponents=\"3\" Format=\"ascii\">" << std::endl;

    std::map<Point, int> V;
    int i = 0;
    indent += indent_unit;

    for (Vertex_iterator it = t.finite_vertices_begin(); it != t.finite_vertices_end(); ++it)
    {
        vtk_file << indent;
        vtk_file << it->point().x() << " " << it->point().y() << " " << it->point().z() << std::endl;
        V[Point(it->point())] = i;
        ++i;
    }

    indent.erase(indent.length() - indent_size, indent_size);
    vtk_file << indent + "</DataArray>" << std::endl;

    indent.erase(indent.length() - indent_size, indent_size);
    vtk_file << indent + "</Points>" << std::endl;

    // Write tetrahedra
    vtk_file << indent << "<Cells>" << std::endl;

    indent += indent_unit;
    vtk_file << indent;
    vtk_file << "<DataArray type=\"Int32\" Name=\"connectivity\" Format=\"ascii\">";
    vtk_file << std::endl;

    indent += indent_unit;
    Cell_iterator it;
    for (it = c3t3.cells_in_complex_begin(); it != c3t3.cells_in_complex_end(); ++it)
    {
        const typename Tr::Cell c(*it);
        const typename Tr::Vertex_handle v0 = c.vertex(0);
        const typename Tr::Vertex_handle v1 = c.vertex(1);
        const typename Tr::Vertex_handle v2 = c.vertex(2);
        const typename Tr::Vertex_handle v3 = c.vertex(3);

        vtk_file << indent;
        vtk_file << V[Point(v0->point())] / MM_TO_M << " ";
        vtk_file << V[Point(v1->point())] / MM_TO_M << " ";
        vtk_file << V[Point(v2->point())] / MM_TO_M << " ";
        vtk_file << V[Point(v3->point())] / MM_TO_M << std::endl;
    }

    indent.erase(indent.length() - indent_size, indent_size);
    vtk_file << indent + "</DataArray>" << std::endl;

    // offsets
    // every element is a four node tetrahedron so all offsets are multiples of 4
    vtk_file << indent;
    vtk_file << "<DataArray type=\"Int32\" Name=\"offsets\" Format=\"ascii\">";
    vtk_file << std::endl;
    i = 4;
    indent += indent_unit;
    for (it = c3t3.cells_in_complex_begin(); it != c3t3.cells_in_complex_end(); ++it)
    {
        vtk_file << indent << i << std::endl;
        i += 4;
    }
    indent.erase(indent.length() - indent_size, indent_size);
    vtk_file << indent + "</DataArray>" << std::endl;

    // cell types (type 10 is a 4 node tetrahedron)
    vtk_file << indent;
    vtk_file << "<DataArray type=\"Int32\" Name=\"types\" Format=\"ascii\">";
    vtk_file << std::endl;
    indent += indent_unit;
    for (it = c3t3.cells_in_complex_begin(); it != c3t3.cells_in_complex_end(); ++it)
    {
        vtk_file << indent << "10" << std::endl;
    }
    indent.erase(indent.length() - indent_size, indent_size);
    vtk_file << indent + "</DataArray>" << std::endl;

    indent.erase(indent.length() - indent_size, indent_size);
    vtk_file << indent + "</Cells>" << std::endl;

    // cell data
    // only subdomain index is written here
    vtk_file << indent + "<CellData Scalars=\"scalars\">" << std::endl;
    indent += indent_unit;
    vtk_file << indent + "<DataArray type=\"Int32\" Name=\"subdomain index\" Format=\"ascii\">" << std::endl;

    indent += indent_unit;
    for (it = c3t3.cells_in_complex_begin(); it != c3t3.cells_in_complex_end(); ++it)
    {
        vtk_file << indent << c3t3.subdomain_index(it) << std::endl;
    }

    indent.erase(indent.length() - indent_size, indent_size);
    vtk_file << indent + "</DataArray>" << std::endl;

    indent.erase(indent.length() - indent_size, indent_size);
    vtk_file << indent + "</CellData>" << std::endl;

    indent.erase(indent.length() - indent_size, indent_size);
    vtk_file << indent + "</Piece>" << std::endl;

    indent.erase(indent.length() - indent_size, indent_size);
    vtk_file << indent + "</UnstructuredGrid>" << std::endl;

    indent.erase(indent.length() - indent_size, indent_size);
    vtk_file << "</VTKFile>" << std::endl;

    return true;
}

} // end namespace CGAL

#endif
