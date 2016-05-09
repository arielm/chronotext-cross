#pragma once

#include "math/Rect.h"
#include "gl/Batch.h"
#include "gl/Matrix.h"

#include "tesselator.h"

namespace chr
{
  class Triangulator
  {
  public:
    Triangulator();
    ~Triangulator();

    template<int Orientation = GL_CCW>
    Triangulator& add(const math::Rectf &rect);

    Triangulator& add(const std::vector<std::vector<glm::vec2>> &polygons);
    Triangulator& add(const std::vector<glm::vec2> &polygon);

    Triangulator& process(gl::IndexedVertexBatch<gl::XYZ> &batch, gl::Matrix &matrix, int windingRule = TESS_WINDING_ODD);
    Triangulator& process(gl::IndexedVertexBatch<gl::XYZ.N.RGBA> &batch, gl::Matrix &matrix, const glm::vec4 &color, int windingRule = TESS_WINDING_ODD);

  protected:
    TESStesselator *tess;
    int allocated = 0;
  };
}
