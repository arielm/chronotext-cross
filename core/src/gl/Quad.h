#pragma once

#include "gl/Vertex.h"
#include "math/Rect.h"

namespace chr
{
  namespace gl
  {
    template<int T = XYZ> struct Quad
    {};

    template<> struct Quad<XYZ>
    {
      union
      {
        glm::vec2 position1;

        struct
        {
          float x1, y1;
        };
      };

      union
      {
        glm::vec2 position2;

        struct
        {
          float x2, y2;
        };
      };

      Quad()
      {}

      Quad(float x1, float y1, float x2, float y2)
      :
      x1(x1),
      y1(y1),
      x2(x2),
      y2(y2)
      {}

      Quad(const math::Rectf &rect)
      :
      x1(rect.x1),
      y1(rect.y1),
      x2(rect.x2),
      y2(rect.y2)
      {}

      Quad(const glm::vec2 &position1, const glm::vec2 &position2)
      :
      position1(position1),
      position2(position2)
      {}
    };

    template<> struct Quad<XYZ.UV> : Quad<>
    {
      union
      {
        glm::vec2 coords1;

        struct
        {
          float u1, v1;
        };
      };

      union
      {
        glm::vec2 coords2;

        struct
        {
          float u2, v2;
        };
      };

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

      Quad(const math::Rectf &rect, const glm::vec2 &coords1, const glm::vec2 &coords2)
      :
      Quad<>(rect),
      coords1(coords1),
      coords2(coords2)
      {}

      Quad(const glm::vec2 &position1, const glm::vec2 &position2, const glm::vec2 &coords1, const glm::vec2 &coords2)
      :
      Quad<>(position1, position2),
      coords1(coords1),
      coords2(coords2)
      {}
    };

    template<> struct Quad<XYZ.RGBA> : Quad<>
    {
      union
      {
        glm::vec4 color;

        struct
        {
          float r, g, b, a;
        };
      };

      Quad()
      {}

      Quad(float x1, float y1, float x2, float y2, const glm::vec4 &color)
      :
      Quad<>(x1, y1, x2, y2),
      color(color)
      {}

      Quad(const math::Rectf &rect, const glm::vec4 &color)
      :
      Quad<>(rect),
      color(color)
      {}

      Quad(const glm::vec2 &position1, const glm::vec2 &position2, const glm::vec4 &color)
      :
      Quad<>(position1, position2),
      color(color)
      {}
    };

    template<> struct Quad<XYZ.UV.RGBA> : Quad<XYZ.UV>
    {
      union
      {
        glm::vec4 color;

        struct
        {
          float r, g, b, a;
        };
      };

      Quad()
      {}

      Quad(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2, const glm::vec4 &color)
      :
      Quad<XYZ.UV>(x1, y1, x2, y2, u1, v1, u2, v2),
      color(color)
      {}

      Quad(const math::Rectf &rect, float u1, float v1, float u2, float v2, const glm::vec4 &color)
      :
      Quad<XYZ.UV>(rect, u1, v1, u2, v2),
      color(color)
      {}

      Quad(const math::Rectf &rect, const glm::vec2 &coords1, const glm::vec2 &coords2, const glm::vec4 &color)
      :
      Quad<XYZ.UV>(rect, coords1, coords2),
      color(color)
      {}

      Quad(const glm::vec2 &position1, const glm::vec2 &position2, const glm::vec2 &coords1, const glm::vec2 &coords2, const glm::vec4 &color)
      :
      Quad<XYZ.UV>(position1, position2, coords1, coords2),
      color(color)
      {}
    };

    template<> struct Quad<XYZ.N> : Quad<>
    {
      union
      {
        glm::vec3 normal;

        struct
        {
          float nx, ny, nz;
        };
      };

      Quad()
      {}

      Quad(float x1, float y1, float x2, float y2, const glm::vec3 &normal)
      :
      Quad<>(x1, y1, x2, y2),
      normal(normal)
      {}

      Quad(const math::Rectf &rect, const glm::vec3 &normal)
      :
      Quad<>(rect),
      normal(normal)
      {}

      Quad(const glm::vec2 &position1, const glm::vec2 &position2, const glm::vec3 &normal)
      :
      Quad<>(position1, position2),
      normal(normal)
      {}
    };

    template<> struct Quad<XYZ.N.UV> : Quad<XYZ.N>
    {
      union
      {
        glm::vec2 coords1;

        struct
        {
          float u1, v1;
        };
      };

      union
      {
        glm::vec2 coords2;

        struct
        {
          float u2, v2;
        };
      };

      Quad()
      {}

      Quad(float x1, float y1, float x2, float y2, const glm::vec3 &normal, float u1, float v1, float u2, float v2)
      :
      Quad<XYZ.N>(x1, y1, x2, y2, normal),
      u1(u1),
      v1(v1),
      u2(u2),
      v2(v2)
      {}

      Quad(const math::Rectf &rect, const glm::vec3 &normal, float u1, float v1, float u2, float v2)
      :
      Quad<XYZ.N>(rect, normal),
      u1(u1),
      v1(v1),
      u2(u2),
      v2(v2)
      {}

      Quad(const math::Rectf &rect, const glm::vec3 &normal, const glm::vec2 &coords1, const glm::vec2 &coords2)
      :
      Quad<XYZ.N>(rect, normal),
      coords1(coords1),
      coords2(coords2)
      {}

      Quad(const glm::vec2 &position1, const glm::vec2 &position2, const glm::vec3 &normal, const glm::vec2 &coords1, const glm::vec2 &coords2)
      :
      Quad<XYZ.N>(position1, position2, normal),
      coords1(coords1),
      coords2(coords2)
      {}
    };

    template<> struct Quad<XYZ.N.UV.RGBA> : Quad<XYZ.N.UV>
    {
      union
      {
        glm::vec4 color;

        struct
        {
          float r, g, b, a;
        };
      };

      Quad()
      {}

      Quad(float x1, float y1, float x2, float y2, const glm::vec3 &normal, float u1, float v1, float u2, float v2, const glm::vec4 &color)
      :
      Quad<XYZ.N.UV>(x1, y1, x2, y2, normal, u1, v1, u2, v2),
      color(color)
      {}

      Quad(const math::Rectf &rect, const glm::vec3 &normal, float u1, float v1, float u2, float v2, const glm::vec4 &color)
      :
      Quad<XYZ.N.UV>(rect, normal, u1, v1, u2, v2),
      color(color)
      {}

      Quad(const math::Rectf &rect, const glm::vec3 &normal, const glm::vec2 &coords1, const glm::vec2 &coords2, const glm::vec4 &color)
      :
      Quad<XYZ.N.UV>(rect, normal, coords1, coords2),
      color(color)
      {}

      Quad(const glm::vec2 &position1, const glm::vec2 &position2, const glm::vec3 &normal, const glm::vec2 &coords1, const glm::vec2 &coords2, const glm::vec4 &color)
      :
      Quad<XYZ.N.UV>(position1, position2, normal, coords1, coords2),
      color(color)
      {}
    };
  }
}
