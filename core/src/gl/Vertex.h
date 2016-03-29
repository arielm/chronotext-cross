#pragma once

#define GLM_SWIZZLE
#include <glm/glm.hpp>

namespace chr
{
  namespace gl
  {
    enum
    {
      UV = 1,
      N = 2,
      RGBA
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

      Vertex(const glm::vec3 &point, float u, float v)
      :
      Vertex<>(point.x, point.y, point.z),
      u(u),
      v(v)
      {}

      Vertex(const glm::vec3 &point, const glm::vec2 &coords)
      :
      Vertex<>(point.x, point.y, point.z),
      u(coords.x),
      v(coords.y)
      {}
    };
  }
}
