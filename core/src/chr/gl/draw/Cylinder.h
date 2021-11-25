/*
 * Based on http://www.songho.ca/opengl/gl_cylinder.html
 */

#pragma once

#include "chr/gl/Batch.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      class Cylinder
      {
      public:
        Cylinder& setFrontFace(GLenum mode);
        Cylinder& setColor(const glm::vec4 &color);
        Cylinder& setColor(float r, float g, float b, float a);
        Cylinder& setSectorCount(int count);
        Cylinder& setStackCount(int count);
        Cylinder& setBaseRadius(float radius);
        Cylinder& setTopRadius(float radius);
        Cylinder& setHeight(float height);

        template<int V = XYZ, typename I = GLuint>
        void append(IndexedVertexBatch<V,I> &batch, const Matrix &matrix) const;

      protected:
        GLenum frontFace = chr::CCW;
        glm::vec4 color = {1, 1, 1, 1};
        int sectorCount = 12;
        int stackCount = 2;
        float topRadius = 1;
        float baseRadius = 1;
        float height = 1;

        std::vector<float> getUnitCircleVertices() const;
        std::vector<float> getSideNormals() const;
      };
    }
  }
}
