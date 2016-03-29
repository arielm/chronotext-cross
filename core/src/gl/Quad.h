#pragma once

#include "gl/Vertex.h"
#include "math/Rect.h"

namespace chr
{
  namespace gl
  {
    template<int T = 0> struct Quad
    {};

    template<> struct Quad<0>
    {
      float x1;
      float y1;
      float x2;
      float y2;

      Quad() = default;

      Quad(float x1, float y1, float x2, float y2)
      :
      x1(x1),
      y1(y1),
      x2(x2),
      y2(y2)
      {}

      Quad(const math::Rectf &rect)
      :
      Quad(rect.x1, rect.y1, rect.x2, rect.y2)
      {}

      Quad(const glm::vec2 &point1, const glm::vec2 &point2)
      :
      Quad(point1.x, point1.y, point2.x, point2.y)
      {}
    };

    template<> struct Quad<UV> : Quad<>
    {
      float u1;
      float v1;
      float u2;
      float v2;

      Quad()
      {}

      Quad(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
      :
      Quad<>(x1, y1, x2, y2),
      u1(u1),
      v1(v1),
      u2(u2),
      v2(v2)
      {}
    };
  }
}
