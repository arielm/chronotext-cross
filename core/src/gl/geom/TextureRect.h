#pragma once

#include "gl/Batch.h"
#include "gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    namespace geom
    {
      template<int V = UV, typename I = GLushort> class TextureRect
      {
      public:
        IndexedVertexBatch<V,I> &batch;

        TextureRect(IndexedVertexBatch<V,I> &batch)
        :
        batch(batch)
        {}

        template<int Orientation=GL_CCW>
        void draw(Matrix &matrix, float x = 0, float y = 0)
        {
          float x2 = x + batch.texture.innerWidth;
          float y2 = y + batch.texture.innerHeight;

          float u1 = batch.texture.u1;
          float v1 = batch.texture.v1;
          float u2 = batch.texture.u2;
          float v2 = batch.texture.v2;

          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<UV>(x, y, x2, y2, 0, 0, u2, v2), batch);
        }

        template<int Orientation=GL_CCW>
        void draw(Matrix &matrix, const glm::vec4 &color, float x = 0, float y = 0)
        {
          float x2 = x + batch.texture.innerWidth;
          float y2 = y + batch.texture.innerHeight;

          float u1 = batch.texture.u1;
          float v1 = batch.texture.v1;
          float u2 = batch.texture.u2;
          float v2 = batch.texture.v2;

          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<UV|RGBA>(x, y, x2, y2, 0, 0, u2, v2, color), batch);
        }

        template<int Orientation=GL_CCW>
        inline void drawFromCenter(Matrix &matrix, float x = 0, float y = 0)
        {
          draw<Orientation>(matrix, x - batch.texture.innerWidth * 0.5f, y - batch.texture.innerHeight * 0.5f);
        }

        template<int Orientation=GL_CCW>
        inline void drawFromCenter(Matrix &matrix, const glm::vec4 &color, float x = 0, float y = 0)
        {
          draw<Orientation>(matrix, color, x - batch.texture.innerWidth * 0.5f, y - batch.texture.innerHeight * 0.5f);
        }

        template<int Orientation=GL_CCW>
        void drawInRect(Matrix &matrix, const math::Rectf &rect, float ox = 0, float oy = 0)
        {
          float width = batch.texture.innerWidth;
          float height = batch.texture.innerHeight;

          float x1 = rect.x1;
          float y1 = rect.y1;
          float x2 = rect.x2;
          float y2 = rect.y2;

          float u1 = (x1 - ox) / width;
          float v1 = (y1 - oy) / height;
          float u2 = (x2 - ox) / width;
          float v2 = (y2 - oy) / height;

          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<V>(x1, y1, x2, y2, u1, v1, u2, v2), batch);
        }

        template<int Orientation=GL_CCW>
        void drawInRect(Matrix &matrix, const math::Rectf &rect, const glm::vec4 &color, float ox = 0, float oy = 0)
        {
          float width = batch.texture.innerWidth;
          float height = batch.texture.innerHeight;

          float x1 = rect.x1;
          float y1 = rect.y1;
          float x2 = rect.x2;
          float y2 = rect.y2;

          float u1 = (x1 - ox) / width;
          float v1 = (y1 - oy) / height;
          float u2 = (x2 - ox) / width;
          float v2 = (y2 - oy) / height;

          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<V>(x1, y1, x2, y2, u1, v1, u2, v2, color), batch);
        }
      };
    }
  }
}
