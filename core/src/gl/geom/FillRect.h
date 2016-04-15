#pragma once

#include "gl/Batch.h"
#include "gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    namespace geom
    {
      template<int V = 0, typename I = GLushort>
      class FillRect
      {
      public:
        IndexedVertexBatch<V,I> &batch;

        FillRect(IndexedVertexBatch<V,I> &batch)
        :
        batch(batch)
        {}

        template<int Orientation=GL_CCW>
        inline void draw(Matrix &matrix, const math::Rectf &rect) const
        {
          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<V>(rect), batch);
        }

        template<int Orientation=GL_CCW>
        inline void draw(Matrix &matrix, float x, float y, float width, float height) const
        {
          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<V>(math::Rectf(x, y, width, height)), batch);
        }
        
        void draw(const math::Rectf &rect) const
        {
          batch.addVertex(rect.x1, rect.y1);
          batch.addVertex(rect.x1, rect.y2);
          batch.addVertex(rect.x2, rect.y2);
          batch.addVertex(rect.x2, rect.y1);

          batch.addIndices(0, 3, 2, 2, 1, 0);
          batch.incrementIndices(4);
        }

        inline void draw(float x, float y, float width, float height) const
        {
          draw(math::Rectf(x, y, width, height));
        }
      };
    }
  }
}
