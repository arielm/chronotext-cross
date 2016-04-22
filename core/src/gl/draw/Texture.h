#pragma once

#include "gl/Batch.h"
#include "gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      template<int V = XYZ.UV, typename I = GLushort>
      class Texture
      {
      public:
        IndexedVertexBatch<V,I> &batch;

        Texture(IndexedVertexBatch<V,I> &batch)
        :
        batch(batch)
        {}

        template<int Orientation=GL_CCW, typename... Args>
        void fill(Matrix &matrix, float x, float y, Args&&... args)
        {
          float x2 = x + batch.texture.innerWidth;
          float y2 = y + batch.texture.innerHeight;

          float u1 = batch.texture.u1;
          float v1 = batch.texture.v1;
          float u2 = batch.texture.u2;
          float v2 = batch.texture.v2;

          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<V>(x, y, x2, y2, u1, v1, u2, v2, std::forward<Args>(args)...), batch);
        }

        template<int Orientation=GL_CCW, typename... Args>
        void fill(float x, float y, Args&&... args)
        {
          float x2 = x + batch.texture.innerWidth;
          float y2 = y + batch.texture.innerHeight;

          float u1 = batch.texture.u1;
          float v1 = batch.texture.v1;
          float u2 = batch.texture.u2;
          float v2 = batch.texture.v2;

          batch.addVertex( x,  y, 0, u1, v1, std::forward<Args>(args)...);
          batch.addVertex( x, y2, 0, u1, v2, std::forward<Args>(args)...);
          batch.addVertex(x2, y2, 0, u2, v2, std::forward<Args>(args)...);
          batch.addVertex(x2,  y, 0, u2, v1, std::forward<Args>(args)...);

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

        template<int Orientation=GL_CCW, typename... Args>
        inline void fillFromCenter(Matrix &matrix, float x, float y, Args&&... args)
        {
          fill<Orientation>(matrix, x - batch.texture.innerWidth * 0.5f, y - batch.texture.innerHeight * 0.5f, std::forward<Args>(args)...);
        }

        template<int Orientation=GL_CCW>
        void fillRect(Matrix &matrix, const math::Rectf &rect, float ox = 0, float oy = 0)
        {
          float width = batch.texture.innerWidth;
          float height = batch.texture.innerHeight;

          float u1 = (rect.x1 - ox) / width;
          float v1 = (rect.y1 - oy) / height;
          float u2 = (rect.x2 - ox) / width;
          float v2 = (rect.y2 - oy) / height;

          matrix.addTransformedQuad<GL_TRIANGLES, Orientation>(Quad<V>(rect, u1, v1, u2, v2), batch);
        }
      };
    }
  }
}
