#pragma once

#include "gl/Batch.h"
#include "gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    namespace geom
    {
      template<int V = 0, typename I = GLushort>
      class TextureRect
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

          batch.addIndices(0, 1, 2, 2, 3, 0);
          batch.incrementIndices(4);
        }

        template<int Orientation=GL_CCW>
        inline void drawFromCenter(Matrix &matrix, float x = 0, float y = 0)
        {
          draw<Orientation>(matrix, x - batch.texture.width * batch.texture.maxU * 0.5f, y - batch.texture.height * batch.texture.maxV * 0.5f);
        }

        template<int Orientation=GL_CCW>
        void draw(float x = 0, float y = 0)
        {
          float x2 = x + batch.texture.width * batch.texture.maxU;
          float y2 = y + batch.texture.height * batch.texture.maxV;

          float u2 = batch.texture.maxU;
          float v2 = batch.texture.maxV;

          batch.addVertex( x,  y, 0,  0,  0);
          batch.addVertex( x, y2, 0,  0, v2);
          batch.addVertex(x2, y2, 0, u2, v2);
          batch.addVertex(x2,  y, 0, u2,  0);

          if (Orientation == GL_CCW)
          {
            batch.addIndices(0, 1, 2, 2, 3, 0);
          }
          else
          {
            batch.addIndices(0, 3, 2, 2, 1, 0);
          }

          batch.incrementIndices(4);
        }

        template<int Orientation=GL_CCW>
        inline void drawFromCenter(float x = 0, float y = 0)
        {
          draw<Orientation>(x - batch.texture.width * batch.texture.maxU * 0.5f, y - batch.texture.height * batch.texture.maxV * 0.5f);
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

          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<UV>(x1, y1, x2, y2, u1, v1, u2, v2), batch);

          if (Orientation == GL_CCW)
          {
            batch.addIndices(0, 1, 2, 2, 3, 0);
          }
          else
          {
            batch.addIndices(0, 3, 2, 2, 1, 0);
          }

          batch.incrementIndices(4);
        }

        template<int Orientation=GL_CCW>
        void drawInRect(const math::Rectf &rect, float ox = 0, float oy = 0)
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

          batch.addVertex(x1, y1, 0, u1, u1);
          batch.addVertex(x1, y2, 0, u1, v2);
          batch.addVertex(x2, y2, 0, u2, v2);
          batch.addVertex(x2, y1, 0, u2, u1);

          if (Orientation == GL_CCW)
          {
            batch.addIndices(0, 1, 2, 2, 3, 0);
          }
          else
          {
            batch.addIndices(0, 3, 2, 2, 1, 0);
          }

          batch.incrementIndices(4);
        }
      };
    }
  }
}
