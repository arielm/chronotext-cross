#pragma once

#include "gl/Batch.h"
#include "gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      class Rect
      {
      public:
        Rect& setFrontFace(GLenum mode);

        Rect& setTextureOffset(const glm::vec2 &offset);
        Rect& setTextureOffset(float x, float y);
        Rect& setTextureScale(float scale);
        Rect& setTextureScale(float scaleX, float scaleY);

        Rect& setColor(const glm::vec4 &color);
        Rect& setColor(float r, float g, float b, float a);

        Rect& setBounds(const math::Rectf &bounds);
        Rect& setBounds(const glm::vec2 &upperLeft, const glm::vec2 &lowerRight);
        Rect& setBounds(float left, float top, float width, float height);

        template<int V = XYZ, typename I = GLushort>
        void append(IndexedVertexBatch<V,I> &batch, const Matrix &matrix) const;

        template<int V = XYZ, typename I = GLushort>
        void append(IndexedVertexBatch<V,I> &batch) const;

      protected:
        GLenum frontFace = CCW;
        glm::vec2 textureOffset;
        glm::vec2 textureScale = { 1, 1 };
        glm::vec4 color = { 1, 1, 1, 1 };
        math::Rectf bounds;

        std::pair<glm::vec2, glm::vec2> getTextureCoords(const gl::Texture &texture) const;
      };
    }
  }
}
