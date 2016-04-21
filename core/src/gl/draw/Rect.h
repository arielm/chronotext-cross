#pragma once

#include "gl/Batch.h"
#include "gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      template<int V = 0, typename I = GLushort>
      class Rect
      {
      public:
        IndexedVertexBatch<V,I> &batch;

        Rect(IndexedVertexBatch<V,I> &batch)
        :
        batch(batch)
        {}

        template<int Orientation=GL_CCW>
        inline void fill(Matrix &matrix, float x1, float y1, float x2, float y2) const
        {
          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<V>(x1, y1, x2, y2), batch);
        }

        template<int Orientation=GL_CCW>
        inline void fill(Matrix &matrix, const math::Rectf &rect) const
        {
          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<V>(rect), batch);
        }

        template<int Orientation=GL_CCW>
        void fill(float x1, float y1, float x2, float y2) const
        {
          batch.addVertex(x1, y1);
          batch.addVertex(x1, y2);
          batch.addVertex(x2, y2);
          batch.addVertex(x2, y1);

          if (Orientation == GL_CCW)
          {
            batch.addIndices(0, 1, 2, 2, 3, 0);
          }
          else
          {
            batch.addIndices(0, 3, 2, 2, 1, 0);
          }
          
          batch.incrementIndices(4);
        }

        template<int Orientation=GL_CCW>
        inline void fill(const math::Rectf &rect) const
        {
          fill<Orientation>(rect.x1, rect.y1, rect.x2, rect.y2);
        }
      };
    }
  }
}
