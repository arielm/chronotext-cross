#pragma once

#include "gl/Batch.h"
#include "gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      class Rect
      {
      public:
        Rect& setBounds(const math::Rectf &bounds);
        Rect& setBounds(const glm::vec2 &upperLeft, const glm::vec2 &lowerRight);
        Rect& setBounds(float left, float top, float width, float height);

        Rect& setColor(const glm::vec4 &color);
        Rect& setColor(float r, float g, float b, float a);

        template<int Orientation = GL_CCW, int V = XYZ, typename I = GLushort>
        void append(IndexedVertexBatch<V,I> &batch, Matrix &matrix) const;

        template<int Orientation = GL_CCW, int V = XYZ, typename I = GLushort>
        void append(IndexedVertexBatch<V,I> &batch) const;

      protected:
        glm::vec4 color;
        math::Rectf bounds;
      };
    }
  }
}
