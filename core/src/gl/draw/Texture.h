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
        Texture(gl::Texture &texture);

        Texture& setColor(const glm::vec4 &color);
        Texture& setColor(float r, float g, float b, float a);

        template<int Orientation = GL_CCW, int V = XYZ, typename I = GLushort>
        void fill(IndexedVertexBatch<V,I> &batch, Matrix &matrix, float x = 0, float y = 0) const;

        template<int Orientation = GL_CCW, int V = XYZ, typename I = GLushort>
        inline void fillFromCenter(IndexedVertexBatch<V,I> &batch, Matrix &matrix, float x = 0, float y = 0)
        {
          fill<Orientation>(batch, matrix, x - texture.innerWidth * 0.5f, y - texture.innerHeight * 0.5f);
        }

        template<int Orientation = GL_CCW, int V = XYZ, typename I = GLushort>
        void fill(IndexedVertexBatch<V,I> &batch, float x = 0, float y = 0) const;

        template<int Orientation = GL_CCW, int V = XYZ, typename I = GLushort>
        inline void fillFromCenter(IndexedVertexBatch<V,I> &batch, float x, float y)
        {
          fill<Orientation>(batch, x - texture.innerWidth * 0.5f, y - texture.innerHeight * 0.5f);
        }

        template<int Orientation = GL_CCW, int V = XYZ, typename I = GLushort>
        void fillRect(IndexedVertexBatch<V,I> &batch, Matrix &matrix, const math::Rectf &rect, float ox = 0, float oy = 0);

      protected:
        gl::Texture texture;
        glm::vec4 color;
      };
    }
  }
}
