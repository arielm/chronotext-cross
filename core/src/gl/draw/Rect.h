#pragma once

#include "gl/Batch.h"
#include "gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      template<int V = XYZ, typename I = GLushort>
      class Rect
      {
      public:
        IndexedVertexBatch<V,I> &batch;

        Rect(IndexedVertexBatch<V,I> &batch)
        :
        batch(batch)
        {}

        template<int Orientation=GL_CCW, typename... Args>
        inline void fill(Matrix &matrix, float x1, float y1, float x2, float y2, Args&&... args) const
        {
          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<V>(x1, y1, x2, y2, std::forward<Args>(args)...), batch);
        }

        template<int Orientation=GL_CCW, typename... Args>
        inline void fill(Matrix &matrix, const math::Rectf &rect, Args&&... args) const
        {
          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<V>(rect, std::forward<Args>(args)...), batch);
        }

        template<int Orientation=GL_CCW, typename... Args>
        void fill(float x1, float y1, float x2, float y2, Args&&... args) const
        {
          batch.addVertex(x1, y1, 0, std::forward<Args>(args)...);
          batch.addVertex(x1, y2, 0, std::forward<Args>(args)...);
          batch.addVertex(x2, y2, 0, std::forward<Args>(args)...);
          batch.addVertex(x2, y1, 0, std::forward<Args>(args)...);

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

        template<int Orientation=GL_CCW, typename... Args>
        inline void fill(const math::Rectf &rect, Args&&... args) const
        {
          fill<Orientation>(rect.x1, rect.y1, rect.x2, rect.y2, std::forward<Args>(args)...);
        }
      };
    }
  }
}
