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

      Quad(const glm::vec2 &position1, const glm::vec2 &position2)
      :
      Quad(position1.x, position1.y, position2.x, position2.y)
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

      Quad(const math::Rectf &rect, float u1, float v1, float u2, float v2)
      :
      Quad<>(rect),
      u1(u1),
      v1(v1),
      u2(u2),
      v2(v2)
      {}

      Quad(const glm::vec2 &position1, const glm::vec2 &position2, const glm::vec2 &coords1, const glm::vec2 &coords2)
      :
      Quad<>(position1, position2),
      u1(coords1.s),
      v1(coords1.t),
      u2(coords2.s),
      v2(coords2.t)
      {}
    };

    template<> struct Quad<RGBA> : Quad<>
    {
      float r;
      float g;
      float b;
      float a;

      Quad()
      {}

      Quad(float x1, float y1, float x2, float y2, float r, float g, float b, float a)
      :
      Quad<>(x1, y1, x2, y2),
      r(r),
      g(g),
      b(b),
      a(a)
      {}

      Quad(const math::Rectf &rect, float r, float g, float b, float a)
      :
      Quad<>(rect),
      r(r),
      g(g),
      b(b),
      a(a)
      {}

      Quad(const glm::vec2 &position1, const glm::vec2 &position2, const glm::vec4 &color)
      :
      Quad<>(position1, position2),
      r(color.r),
      g(color.g),
      b(color.b),
      a(color.a)
      {}
    };

    template<> struct Quad<UV|RGBA> : Quad<UV>
    {
      float r;
      float g;
      float b;
      float a;

      Quad()
      {}

      Quad(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2, float r, float g, float b, float a)
      :
      Quad<UV>(x1, y1, x2, y2, u1, v1, u2, v2),
      r(r),
      g(g),
      b(b),
      a(a)
      {}

      Quad(const math::Rectf &rect, float u1, float v1, float u2, float v2, float r, float g, float b, float a)
      :
      Quad<UV>(rect, u1, v1, u2, v2),
      r(r),
      g(g),
      b(b),
      a(a)
      {}

      Quad(const glm::vec2 &position1, const glm::vec2 &position2, const glm::vec2 &coords1, const glm::vec2 &coords2, const glm::vec4 &color)
      :
      Quad<UV>(position1, position2, coords1, coords2),
      r(color.r),
      g(color.g),
      b(color.b),
      a(color.a)
      {}
    };
  }
}
