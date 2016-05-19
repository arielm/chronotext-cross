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
class PolyhedronBuilder : public CGAL::Modifier_base<HDS>
{
  typedef typename HDS::Vertex Vertex;
  typedef typename Vertex::Point Point;

  std::vector<double> &coords;
  std::vector<int> &tris;

public:
  PolyhedronBuilder(std::vector<double> &_coords, std::vector<int> &_tris)
  :
  coords(_coords),
  tris(_tris)
  {}

  void operator()(HDS &hds)
  {
    // create a cgal incremental builder
    CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
    B.begin_surface(coords.size() / 3, tris.size() / 3);

    // add the polyhedron vertices
    for(int i=0; i< coords.size(); i += 3)
    {
      B.add_vertex(Point(coords[i], coords[i + 1], coords[i + 2]));
    }

    // add the polyhedron triangles
    for( int i=0; i<(int)tris.size(); i+=3 )
    {
      B.begin_facet();
      B.add_vertex_to_facet( tris[i] );
      B.add_vertex_to_facet( tris[i + 1] );
      B.add_vertex_to_facet( tris[i + 2] );
      B.end_facet();
    }

    // finish up the surface
    B.end_surface();
  }
};
