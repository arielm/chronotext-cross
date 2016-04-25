#pragma once

#include "gl/Batch.h"
#include "gl/Matrix.h"
#include "math/Utils.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      class Circle
      {
      public:
        Circle& setColor(const glm::vec4 &color);
        Circle& setColor(float r, float g, float b, float a);

        Circle& setRadius(float radius);
        Circle& setArc(float a1, float a2);
        Circle& setSegmentLength(float length);

        template<int Orientation = GL_CCW, int V = XYZ, typename I = GLushort>
        void fill(IndexedVertexBatch<V,I> &batch, Matrix &matrix, float x = 0, float y = 0) const;

      protected:
        glm::vec4 color;

        float r = 1;
        float a1 = 0;
        float a2 = TWO_PI;
        float segmentLength = 8;
      };
    }
  }
}
