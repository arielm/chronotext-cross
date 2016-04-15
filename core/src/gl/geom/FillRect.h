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
        inline void draw(Matrix &matrix, float x1, float y1, float x2, float y2) const
        {
          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<V>(x1, y1, x2, y2), batch);
        }

        template<int Orientation=GL_CCW>
        void draw(const math::Rectf &rect) const
        {
          batch.addVertex(rect.x1, rect.y1);
          batch.addVertex(rect.x1, rect.y2);
          batch.addVertex(rect.x2, rect.y2);
          batch.addVertex(rect.x2, rect.y1);

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
        inline void draw(float x1, float y1, float x2, float y2) const
        {
          draw<Orientation>(math::Rectf(glm::vec2(x1, y1), glm::vec2(x2, y2)));
        }
      };
    }
  }
}
