#pragma once

#include "cgal.h"

#include "gl/Batch.h"

class Polyhedron : public CGAL::Polyhedron_3<Kernel>
{
public:
  Polyhedron() : CGAL::Polyhedron_3<Kernel>()
  {}

  void append(chr::gl::VertexBatch<chr::gl::XYZ> &batch) const;
};

typedef Polyhedron::HalfedgeDS HalfedgeDS;
