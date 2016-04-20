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
          float x2 = x + batch.texture.width * batch.texture.maxU;
          float y2 = y + batch.texture.height * batch.texture.maxV;

          float u2 = batch.texture.maxU;
          float v2 = batch.texture.maxV;

          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<UV>(x, y, x2, y2, 0, 0, u2, v2), batch);
        }

        template<int Orientation=GL_CCW>
        void draw(Matrix &matrix, const glm::vec4 &color, float x = 0, float y = 0)
        {
          float x2 = x + batch.texture.width * batch.texture.maxU; // TODO: texture.cleanWidth
          float y2 = y + batch.texture.height * batch.texture.maxV; // TODO: texture.cleanHeight

          float u2 = batch.texture.maxU;
          float v2 = batch.texture.maxV;

          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<UV|RGBA>(x, y, x2, y2, 0, 0, u2, v2, color), batch);
        }

        template<int Orientation=GL_CCW>
        inline void drawFromCenter(Matrix &matrix, float x = 0, float y = 0)
        {
          draw<Orientation>(matrix, x - batch.texture.width * batch.texture.maxU * 0.5f, y - batch.texture.height * batch.texture.maxV * 0.5f);
        }

        template<int Orientation=GL_CCW>
        inline void drawFromCenter(Matrix &matrix, const glm::vec4 &color, float x = 0, float y = 0)
        {
          draw<Orientation>(matrix, color, x - batch.texture.width * batch.texture.maxU * 0.5f, y - batch.texture.height * batch.texture.maxV * 0.5f);
        }

        template<int Orientation=GL_CCW>
        void drawInRect(Matrix &matrix, const math::Rectf &rect, float ox = 0, float oy = 0)
        {
          float width = batch.texture.width * batch.texture.maxU;
          float height = batch.texture.height * batch.texture.maxV;

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
          float width = batch.texture.width * batch.texture.maxU;
          float height = batch.texture.height * batch.texture.maxV;

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
