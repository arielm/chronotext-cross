#pragma once

#include "gl/Batch.h"
#include "gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      class Texture
      {
      public:
        Texture() = default;

        Texture& setFrontFace(GLenum mode);

        Texture& setBounds(const math::Rectf &bounds);
        Texture& setBounds(const glm::vec2 &upperLeft, const glm::vec2 &lowerRight);
        Texture& setBounds(float left, float top, float width, float height);

        Texture& setAnchor(const glm::vec2 &anchor);
        Texture& setAnchor(float x, float y);

        Texture& setOffset(const glm::vec2 &offset);
        Texture& setOffset(float x, float y);

        Texture& setScale(float scale);

        Texture& setColor(const glm::vec4 &color);
        Texture& setColor(float r, float g, float b, float a);

        template<int V = XYZ, typename I = GLushort>
        void append(IndexedVertexBatch<V,I> &batch, Matrix &matrix, float x = 0, float y = 0) const;

        template<int V = XYZ, typename I = GLushort>
        void append(IndexedVertexBatch<V,I> &batch, float x = 0, float y = 0) const;

      protected:
        GLenum frontFace = GL_CCW;
        glm::vec4 color;
        glm::vec2 anchor;

        glm::vec2 offset;
        float scale = 1;

        math::Rectf bounds;
        bool hasBounds;

        std::tuple<math::Rectf, glm::vec2, glm::vec2> computeTexturedQuad(const gl::Texture &texture, float x, float y) const;
      };
    }
  }
}
