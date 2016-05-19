#pragma once

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Nef_polyhedron_3<Kernel> Nef_polyhedron;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::HalfedgeDS HalfedgeDS;
typedef Polyhedron::Halfedge_handle Halfedge_handle;
typedef Polyhedron::Halfedge_iterator Halfedge_iterator;
typedef Polyhedron::size_type size_type;

template<class HDS>
class MeshCreator : public CGAL::Modifier_base<HDS>
{
  typedef typename HDS::Vertex Vertex;
  typedef typename Vertex::Point Point;

private:
  int _numVertices;
  int _numFacets;
  int _numHalfedges;
  std::vector<double> _vertices;
  std::vector<int> _triangles;

public:
  MeshCreator(std::vector<double> & vertices, std::vector<int> & triangles, int numHalfedges)
  :
  _numHalfedges(numHalfedges)
  {
    _vertices = vertices;
    _triangles = triangles;
    _numVertices = vertices.size() / 3;
    _numFacets = triangles.size() / 3;
  }

  void operator()(HDS &hds)
  {
    CGAL::Polyhedron_incremental_builder_3<HDS> mesh(hds);
    mesh.begin_surface(_numVertices, _numFacets, _numHalfedges);
    for (size_t i = 0; i < _numVertices; ++i)
    {
      mesh.add_vertex(Point(_vertices[i * 3], _vertices[i * 3 + 1], _vertices[i * 3 + 2]));
    }
    for (size_t i = 0; i < _numFacets; ++i)
    {
      mesh.begin_facet();
      mesh.add_vertex_to_facet(_triangles[i * 3]);
      mesh.add_vertex_to_facet(_triangles[i * 3 + 1]);
      mesh.add_vertex_to_facet(_triangles[i * 3 + 2]);
      mesh.end_facet();
    }
    mesh.end_surface();
  }
};
