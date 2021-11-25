#pragma once

#include "chr/gl/Batch.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      class Torus
      {
      public:
        Torus& setFrontFace(GLenum mode);
        Torus& setColor(const glm::vec4 &color);
        Torus& setColor(float r, float g, float b, float a);
        Torus& setSliceCount(int count);
        Torus& setLoopCount(int count);
        Torus& setInnerRadius(float radius);
        Torus& setOuterRadius(float radius);

        template<int V = XYZ, typename I = GLuint>
        void append(IndexedVertexBatch<V,I> &batch, const Matrix &matrix) const;

      protected:
        GLenum frontFace = chr::CCW;
        glm::vec4 color = {1, 1, 1, 1};
        int sliceCount = 5;
        int loopCount = 15;
        float innerRadius = 1;
        float outerRadius = 4;
      };
    }
  }
}
