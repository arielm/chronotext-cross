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
        void draw(Matrix &matrix, const math::Rectf &rect)
        {
          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<V>(rect), batch);
        }

      protected:
      };
    }
  }
}
