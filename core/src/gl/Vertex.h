#pragma once

#include "glm.h"

namespace chr
{
  namespace gl
  {
    enum VertexType
    {
      UV = 1,
      N = 2,
      RGBA = 4
    };

    // ---

    template<int T = 0> struct Vertex
    {};

    template<> struct Vertex<0>
    {
      float x;
      float y;
      float z;

      Vertex() = default;

      Vertex(float x, float y, float z = 0)
      :
      x(x),
      y(y),
      z(z)
      {}

      Vertex(const glm::vec3 &position)
      :
      Vertex(position.x, position.y, position.z)
      {}

      Vertex(const glm::vec2 &position)
      :
      Vertex(position.x, position.y)
      {}
    };

    template<> struct Vertex<UV> : Vertex<>
    {
      float u;
      float v;

      Vertex()
      {}

      Vertex(float x, float y, float z, float u, float v)
      :
      Vertex<>(x, y, z),
      u(u),
      v(v)
      {}

      Vertex(const glm::vec3 &position, float u, float v)
      :
      Vertex<>(position),
      u(u),
      v(v)
      {}

      Vertex(const glm::vec3 &position, const glm::vec2 &coords)
      :
      Vertex<>(position),
      u(coords.x),
      v(coords.y)
      {}
    };

    template<> struct Vertex<RGBA> : Vertex<>
    {
      float r;
      float g;
      float b;
      float a;

      Vertex()
      {}

      Vertex(float x, float y, float z, float r, float g, float b, float a)
      :
      Vertex<>(x, y, z),
      r(r),
      g(g),
      b(b),
      a(a)
      {}

      Vertex(const glm::vec3 &position, float r, float g, float b, float a)
      :
      Vertex<>(position),
      r(r),
      g(g),
      b(b),
      a(a)
      {}

      Vertex(const glm::vec3 &position, const glm::vec4 &color)
      :
      Vertex<>(position),
      r(color.r),
      g(color.g),
      b(color.b),
      a(color.a)
      {}
    };

    template<> struct Vertex<UV|RGBA> : Vertex<UV>
    {
      float r;
      float g;
      float b;
      float a;

      Vertex()
      {}

      Vertex(float x, float y, float z, float u, float v, float r, float g, float b, float a)
      :
      Vertex<UV>(x, y, z, u, v),
      r(r),
      g(g),
      b(b),
      a(a)
      {}

      Vertex(const glm::vec3 &position, float u, float v, float r, float g, float b, float a)
      :
      Vertex<UV>(position, u, v),
      r(r),
      g(g),
      b(b),
      a(a)
      {}

      Vertex(const glm::vec3 &position, const glm::vec2 &coords, const glm::vec4 &color)
      :
      Vertex<UV>(position, coords),
      r(color.r),
      g(color.g),
      b(color.b),
      a(color.a)
      {}
    };
  }
}
