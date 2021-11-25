/*
 * Based on http://www.songho.ca/opengl/gl_sphere.html
 */

#pragma once

#include "chr/gl/Batch.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      class Sphere
      {
      public:
        Sphere& setFrontFace(GLenum mode);
        Sphere& setColor(const glm::vec4 &color);
        Sphere& setColor(float r, float g, float b, float a);
        Sphere& setSectorCount(int count);
        Sphere& setStackCount(int count);
        Sphere& setRadius(float radius);

        template<int V = XYZ, typename I = GLuint>
        void append(IndexedVertexBatch<V,I> &batch, const Matrix &matrix) const;

      protected:
        GLenum frontFace = chr::CCW;
        glm::vec4 color = {1, 1, 1, 1};
        int sectorCount = 12;
        int stackCount = 6;
        float radius = 1;
      };
    }
  }
}
