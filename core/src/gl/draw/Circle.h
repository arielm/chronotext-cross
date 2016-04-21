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
      class Circle
      {
      public:
        IndexedVertexBatch<V,I> &batch;

        Circle(IndexedVertexBatch<V,I> &batch)
        :
        batch(batch)
        {}

        template<int Orientation=GL_CCW>
        void fill(Matrix &matrix, float x, float y, float r, float a0 = 0, float a1 = TWO_PI, float dd = 8) const
        {
          float aa = fabsf(a1 - a0);
          int n = ceilf(aa * r / dd) + 1;

          // TODO: batch.reserve(n + 1);

          batch.addVertex(matrix.transformPoint(x, y));

          for (int i = 0; i < n; i++)
          {
            float d = fmin(aa, i * dd / r);
            float xx = x + sinf(a0 + d) * r;
            float yy = y + cosf(a0 + d) * r;

            batch.addVertex(matrix.transformPoint(xx, yy));

            if (i < n - 1)
            {
              if (Orientation == GL_CCW)
              {
                batch.addIndices(0, i + 1, i + 2);
              }
              else
              {
                batch.addIndices(i + 1, 0, i + 2);
              }
            }
          }

          batch.incrementIndices(n + 1);
        }

        template<int Orientation=GL_CCW>
        void fill(float x, float y, float r, float a0 = 0, float a1 = TWO_PI, float dd = 8) const
        {
          float aa = fabsf(a1 - a0);
          int n = ceilf(aa * r / dd) + 1;

          // TODO: batch.reserve(n + 1);

          batch.addVertex(x, y);

          for (int i = 0; i < n; i++)
          {
            float d = fmin(aa, i * dd / r);
            float xx = x + sinf(a0 + d) * r;
            float yy = y + cosf(a0 + d) * r;

            batch.addVertex(xx, yy);

            if (i < n)
            {
              if (Orientation == GL_CCW)
              {
                batch.addIndices(i + 1, 0, i + 2);
              }
              else
              {
                batch.addIndices(0, i + 1, i + 2);
              }
            }
          }
          
          batch.incrementIndices(n + 1);
        }
      };
    }
  }
}
