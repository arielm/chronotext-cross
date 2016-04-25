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
        Circle& setColor(const glm::vec4 &color)
        {
          this->color = color;
          return *this;
        }

        Circle& setColor(float r, float g, float b, float a)
        {
          color = { r, g, b, a };
          return *this;
        }

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
