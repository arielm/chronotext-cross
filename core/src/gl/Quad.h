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

    struct BaseVertex
    {
      float x;
      float y;
      float z;

      BaseVertex() = default;

      BaseVertex(float x, float y, float z)
      :
      x(x),
      y(y),
      z(z)
      {}
    };

    template<int T = XY> struct Vertex
    {};

    template<> struct Vertex<XY> : BaseVertex
    {
      Vertex()
      {}

      Vertex(float x, float y, float z)
      :
      BaseVertex(x, y, z)
      {}

      Vertex(const glm::vec3 &point)
      :
      BaseVertex(point.x, point.y, point.z)
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

    struct BaseQuad
    {
      float x1;
      float y1;
      float x2;
      float y2;

      BaseQuad() = default;

      BaseQuad(float x1, float y1, float x2, float y2)
      :
      x1(x1),
      y1(y1),
      x2(x2),
      y2(y2)
      {}
    };

    template<int T = XY> struct Quad
    {};

    template<> struct Quad<XY> : BaseQuad
    {
      Quad()
      {}

      Quad(float x1, float y1, float x2, float y2)
      :
      BaseQuad(x1, y1, x2, y2)
      {}

      Quad(const math::Rectf &rect)
      :
      BaseQuad(rect.x1, rect.y1, rect.x2, rect.y2)
      {}

      Quad(const glm::vec2 &point1, const glm::vec2 &point2)
      :
      BaseQuad(point1.x, point1.y, point2.x, point2.y)
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
