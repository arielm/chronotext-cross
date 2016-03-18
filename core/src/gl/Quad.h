#pragma once

#include "math/Rect.h"

namespace chr
{
  namespace gl
  {
    enum
    {
      XY = 0,
      UV = 1,
      N = 2
    };

    // ---

    template<int T = XY> struct Vertex
    {};

    template<> struct Vertex<XY>
    {
      float x;
      float y;
      float z;

      Vertex() = default;

      Vertex(float x, float y, float z)
      :
      x(x),
      y(y),
      z(z)
      {}

      Vertex(const glm::vec3 &point)
      :
      Vertex(point.x, point.y, point.z)
      {}

      Vertex(float x, float y)
      :
      x(x),
      y(y),
      z(0)
      {}

      Vertex(const glm::vec2 &point)
      :
      Vertex(point.x, point.y)
      {}
    };

    template<> struct Vertex<UV> : Vertex<XY>
    {
      float u;
      float v;

      Vertex()
      {}

      Vertex(float x, float y, float z, float u, float v)
      :
      Vertex<XY>(x, y, z),
      u(u),
      v(v)
      {}

      Vertex(const glm::vec3 &point, float u, float v)
      :
      Vertex<XY>(point.x, point.y, point.z),
      u(u),
      v(v)
      {}

      Vertex(const glm::vec3 &point, const glm::vec2 &coords)
      :
      Vertex<XY>(point.x, point.y, point.z),
      u(coords.x),
      v(coords.y)
      {}
    };

    // ---

    template<int T = XY> struct Quad
    {};

    template<> struct Quad<XY>
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

    template<> struct Quad<UV> : Quad<XY>
    {
      float u1;
      float v1;
      float u2;
      float v2;

      Quad()
      {}

      Quad(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
      :
      Quad<XY>(x1, y1, x2, y2),
      u1(u1),
      v1(v1),
      u2(u2),
      v2(v2)
      {}
    };
  }
}
