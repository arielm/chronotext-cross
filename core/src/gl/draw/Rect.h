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
        Rect& setColor(const glm::vec4 &color);
        Rect& setColor(float r, float g, float b, float a);

        template<int Orientation = GL_CCW, int V = XYZ, typename I = GLushort>
        void fill(IndexedVertexBatch<V,I> &batch, Matrix &matrix, float x1, float y1, float x2, float y2) const;

        template<int Orientation = GL_CCW, int V = XYZ, typename I = GLushort>
        inline void fill(IndexedVertexBatch<V,I> &batch, Matrix &matrix, const math::Rectf &rect) const
        {
          fill<Orientation>(batch, matrix, rect.x1, rect.y1, rect.x2, rect.y2);
        };

        template<int Orientation = GL_CCW, int V = XYZ, typename I = GLushort>
        void fill(IndexedVertexBatch<V,I> &batch, float x1, float y1, float x2, float y2) const;

        template<int Orientation = GL_CCW, int V = XYZ, typename I = GLushort>
        inline void fill(IndexedVertexBatch<V,I> &batch, const math::Rectf &rect) const
        {
          fill<Orientation>(batch, rect.x1, rect.y1, rect.x2, rect.y2);
        };

      protected:
        glm::vec4 color;
      };
    }
  }
}
