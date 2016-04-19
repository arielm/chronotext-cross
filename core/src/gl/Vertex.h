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
      union
      {
        glm::vec3 position;

        struct
        {
          float x, y, z;
        };
      };

      Vertex()
      {}

      Vertex(float x, float y, float z = 0)
      :
      x(x),
      y(y),
      z(z)
      {}

      Vertex(const glm::vec3 &position)
      :
      position(position)
      {}
    };

    template<> struct Vertex<UV> : Vertex<>
    {
      union
      {
        glm::vec2 coords;

        struct
        {
          float u, v;
        };
      };

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
      coords(coords)
      {}
    };

    template<> struct Vertex<RGBA> : Vertex<>
    {
      union
      {
        glm::vec4 color;

        struct
        {
          float r, g, b, a;
        };
      };

      Vertex()
      {}

      Vertex(float x, float y, float z, const glm::vec4 &color)
      :
      Vertex<>(x, y, z),
      color(color)
      {}

      Vertex(const glm::vec3 &position, const glm::vec4 &color)
      :
      Vertex<>(position),
      color(color)
      {}
    };

    template<> struct Vertex<UV|RGBA> : Vertex<UV>
    {
      union
      {
        glm::vec4 color;

        struct
        {
          float r, g, b, a;
        };
      };

      Vertex()
      {}

      Vertex(float x, float y, float z, float u, float v, const glm::vec4 &color)
      :
      Vertex<UV>(x, y, z, u, v),
      color(color)
      {}

      Vertex(const glm::vec3 &position, float u, float v, const glm::vec4 &color)
      :
      Vertex<UV>(position, u, v),
      color(color)
      {}

      Vertex(const glm::vec3 &position, const glm::vec2 &coords, const glm::vec4 &color)
      :
      Vertex<UV>(position, coords),
      color(color)
      {}
    };
  }
}
