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
        Circle& setFrontFace(GLenum mode);

        Circle& setTextureOffset(const glm::vec2 &offset);
        Circle& setTextureOffset(float x, float y);
        Circle& setTextureScale(float scale);

        Circle& setColor(const glm::vec4 &color);
        Circle& setColor(float r, float g, float b, float a);

        Circle& setRadius(float radius);
        Circle& setArc(float a1, float a2);
        Circle& setSegmentLength(float length);

        template<int V = XYZ, typename I = GLushort>
        void append(IndexedVertexBatch<V,I> &batch, Matrix &matrix, float x = 0, float y = 0) const;

      protected:
        GLenum frontFace = GL_CCW;
        glm::vec2 textureOffset;
        float textureScale = 1;
        glm::vec4 color = { 1, 1, 1, 1 };

        float r = 1;
        float a1 = 0;
        float a2 = TWO_PI;
        float segmentLength = 8;

        glm::vec2 getTextureCoords(const gl::Texture &texture, float x, float y) const;
      };
    }
  }
}
