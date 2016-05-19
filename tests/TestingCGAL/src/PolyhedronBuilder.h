#pragma once

#include "cgal.h"
#include "gl.h"
#include "gl/Vertex.h"

template<class HDS>
class PolyhedronBuilder : public CGAL::Modifier_base<HDS>
{
  typedef typename HDS::Vertex Vertex;
  typedef typename Vertex::Point Point;

  std::vector<chr::gl::Vertex<chr::gl::XYZ>> &vertices;
  std::vector<GLushort> &indices;

public:
  PolyhedronBuilder(std::vector<chr::gl::Vertex<chr::gl::XYZ>> &vertices, std::vector<GLushort> &indices)
  :
  vertices(vertices),
  indices(indices)
  {}

  void operator()(HDS &hds)
  {
    // create a cgal incremental builder
    CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
    B.begin_surface(vertices.size(), indices.size() / 3);

    // add the polyhedron vertices
    for (const auto &vertex : vertices)
    {
      B.add_vertex(Point(vertex.x, vertex.y, vertex.z));
    }

    // add the polyhedron triangles
    for (int i = 0; i < indices.size(); i += 3)
    {
      B.begin_facet();
      B.add_vertex_to_facet(indices[i]);
      B.add_vertex_to_facet(indices[i + 1]);
      B.add_vertex_to_facet(indices[i + 2]);
      B.end_facet();
    }

    // finish up the surface
    B.end_surface();
  }
};
