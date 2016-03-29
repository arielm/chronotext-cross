#pragma once

#include "gl/ShaderProgram.h"
#include "gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    class QuadBuffer
    {
    public:
      GLint positionLocation;
      GLint colorLocation;
      GLint mvpMatrixLocation;

      QuadBuffer();
      void shutdown();

      void draw(float x1, float y1, float x2, float y2);
      inline void draw(const math::Rectf &rect) { draw(rect.x1, rect.y1, rect.x2, rect.y2); }

      template<int Orientation>
      void draw(const Matrix &matrix, float x1, float y1, float x2, float y2)
      {
        vertices.clear();
        matrix.addTransformedQuad<GL_TRIANGLE_STRIP, Orientation>(Quad<>(x1, y1, x2, y2), vertices);
        apply();
      }

      template<int Orientation>
      void draw(const Matrix &matrix, const math::Rectf &rect)
      {
        vertices.clear();
        matrix.addTransformedQuad<GL_TRIANGLE_STRIP, Orientation>(Quad<>(rect), vertices);
        apply();
      }

      void setShader(ShaderProgram &shader)
      {
        shader.use(); // XXX

        mvpMatrixLocation = shader.mvpMatrixLocation;
        positionLocation = shader.positionLocation;
        colorLocation = shader.colorLocation;
      }

    protected:
      std::vector<Vertex<>> vertices;
      uint32_t vertexVBOId = 0;

      void apply();
    };
  }
}
