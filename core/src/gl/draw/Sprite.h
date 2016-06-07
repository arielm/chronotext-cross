#pragma once

#include "gl/Batch.h"
#include "gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      class Sprite
      {
      public:
        Sprite() = default;

        Sprite& setFrontFace(GLenum mode);

        Sprite& setColor(const glm::vec4 &color);
        Sprite& setColor(float r, float g, float b, float a);

        Sprite& setAnchor(const glm::vec2 &anchor);
        Sprite& setAnchor(float x, float y);

        template<int V = XYZ, typename I = GLushort>
        void append(IndexedVertexBatch<V,I> &batch, Matrix &matrix, float x = 0, float y = 0) const;

        template<int V = XYZ, typename I = GLushort>
        void append(IndexedVertexBatch<V,I> &batch, float x = 0, float y = 0) const;

      protected:
        GLenum frontFace = CCW;
        glm::vec4 color = { 1, 1, 1, 1 };
        glm::vec2 anchor;

        std::tuple<math::Rectf, glm::vec2, glm::vec2> getTextureQuad(const gl::Texture &texture, float x, float y) const;
      };
    }
  }
}
