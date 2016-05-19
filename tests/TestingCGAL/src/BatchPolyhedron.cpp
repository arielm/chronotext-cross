#include "BatchPolyhedron.h"

using namespace chr::gl;

void Polyhedron::append(VertexBatch<XYZ> &batch) const
{
  for (Facet_const_iterator f = facets_begin(); f != facets_end(); ++f)
  {
    Facet::Halfedge_around_facet_const_circulator hbegin = f->facet_begin(), h = hbegin;

    do
    {
      const Point& pt = h->vertex()->point();
      double x = CGAL::to_double(pt[0]);
      double y = CGAL::to_double(pt[1]);
      double z = CGAL::to_double(pt[2]);
      batch.addVertex(x, y, z);
    }
    while (++h != hbegin);
  }
}
