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

        Circle& setRadius(float radius)
        {
          r = radius;
          return *this;
        }

        Circle& setArc(float a1, float a2)
        {
          this->a1 = a1;
          this->a2 = a2;
          return *this;
        }

        Circle& setSegmentLength(float length)
        {
          segmentLength = length;
          return *this;
        }

        template<int Orientation=GL_CCW, typename... Args>
        void fill(Matrix &matrix, float x, float y, Args&&... args) const
        {
          float aa = fabsf(a2 - a1);
          int n = ceilf(aa * r / segmentLength) + 1;

          // TODO: batch.reserve(n + 1);

          batch.addVertex(matrix.transformPoint(x, y), std::forward<Args>(args)...);

          for (int i = 0; i < n; i++)
          {
            float d = fmin(aa, i * segmentLength / r);
            float xx = x + sinf(a1 + d) * r;
            float yy = y + cosf(a1 + d) * r;

            batch.addVertex(matrix.transformPoint(xx, yy), std::forward<Args>(args)...);

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

      protected:
        float r = 1;
        float a1 = 0;
        float a2 = TWO_PI;
        float segmentLength = 8;
      };
    }
  }
}
