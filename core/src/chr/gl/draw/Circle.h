#pragma once

#include "chr/gl/Batch.h"
#include "chr/gl/Matrix.h"

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
        Circle& setTextureScale(float scaleX, float scaleY);

        Circle& setColor(const glm::vec4 &color);
        Circle& setColor(float r, float g, float b, float a);

        Circle& setRadius(float radius);
        Circle& setArc(float a1, float a2);
        Circle& setSegmentLength(float length);

        template<int V = XYZ, typename I = GLushort>
        void append(IndexedVertexBatch<V,I> &batch, const Matrix &matrix, float x = 0, float y = 0) const;

        template<int V = XYZ, typename I = GLushort>
        inline void append(IndexedVertexBatch<V,I> &batch, const Matrix &matrix, const glm::vec2 &position) const
        {
          append(batch, matrix, position.x, position.y);
        };

      protected:
        GLenum frontFace = CCW;
        glm::vec2 textureOffset;
        glm::vec2 textureScale = { 1, 1 };
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
