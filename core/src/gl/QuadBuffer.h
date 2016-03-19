#pragma once

#include "gl/Utils.h"
#include "math/Rect.h"

namespace chr
{
  namespace gl
  {
    class QuadBuffer
    {
    public:
      GLint positionLocation;
      GLint colorLocation;
      GLint matrixLocation;

      QuadBuffer();
      void shutdown();

      void draw(float x1, float y1, float x2, float y2);
      inline void draw(const math::Rectf &rect) { draw(rect.x1, rect.y1, rect.x2, rect.y2); }

      template<int Orientation = GL_CCW>
      void draw(const glm::mat4 &matrix, float x1, float y1, float x2, float y2)
      {
        vertices.clear();
        addTransformedQuad<GL_TRIANGLE_STRIP>(matrix, Quad<>(x1, y1, x2, y2), vertices, Orientation);
        apply();
      }

      template<int Orientation = GL_CCW>
      void draw(const glm::mat4 &matrix, const math::Rectf &rect)
      {
        vertices.clear();
        addTransformedQuad<GL_TRIANGLE_STRIP>(matrix, Quad<>(rect), vertices, Orientation);
        apply();
      }

      void setMatrix(const glm::mat4 &matrix);
      void setColor(float r, float g, float b, float a);
      void setColor(const glm::vec4 &color);

      template <typename T>
      void setShader(T &shader)
      {
        shader.use();

        positionLocation = shader.positionLocation;
        colorLocation = shader.colorLocation;
        matrixLocation = shader.matrixLocation;
      }

    protected:
      std::vector<Vertex<>> vertices;
      uint32_t vertexVBOId = 0;

      void apply();
    };
  }
}
