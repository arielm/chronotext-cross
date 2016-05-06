#include "gl/draw/Texture.h"

using namespace chr::math;

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      Texture& Texture::setColor(const glm::vec4 &color)
      {
        this->color = color;
        return *this;
      }

      Texture& Texture::setColor(float r, float g, float b, float a)
      {
        color = { r, g, b, a };
        return *this;
      }

      // ---

      template <>
      void Texture::fill<GL_CCW>(IndexedVertexBatch<XYZ.UV, GLushort> &batch, Matrix &matrix, float x, float y) const
      {
        float x2 = x + batch.texture.innerWidth;
        float y2 = y + batch.texture.innerHeight;

        float u1 = batch.texture.u1;
        float v1 = batch.texture.v1;
        float u2 = batch.texture.u2;
        float v2 = batch.texture.v2;

        matrix.addTransformedQuad<GL_TRIANGLES, GL_CCW>(Quad<XYZ.UV>(x, y, x2, y2, u1, v1, u2, v2), batch);
      }

      template <>
      void Texture::fill<GL_CW>(IndexedVertexBatch<XYZ.UV, GLushort> &batch, Matrix &matrix, float x, float y) const
      {
        float x2 = x + batch.texture.innerWidth;
        float y2 = y + batch.texture.innerHeight;

        float u1 = batch.texture.u1;
        float v1 = batch.texture.v1;
        float u2 = batch.texture.u2;
        float v2 = batch.texture.v2;

        matrix.addTransformedQuad<GL_TRIANGLES, GL_CW>(Quad<XYZ.UV>(x, y, x2, y2, u1, v1, u2, v2), batch);
      }

      template <>
      void Texture::fill<GL_CCW>(IndexedVertexBatch<XYZ.UV.RGBA, GLushort> &batch, Matrix &matrix, float x, float y) const
      {
        float x2 = x + batch.texture.innerWidth;
        float y2 = y + batch.texture.innerHeight;

        float u1 = batch.texture.u1;
        float v1 = batch.texture.v1;
        float u2 = batch.texture.u2;
        float v2 = batch.texture.v2;

        matrix.addTransformedQuad<GL_TRIANGLES, GL_CCW>(Quad<XYZ.UV.RGBA>(x, y, x2, y2, u1, v1, u2, v2, color), batch);
      }

      template <>
      void Texture::fill<GL_CW>(IndexedVertexBatch<XYZ.UV.RGBA, GLushort> &batch, Matrix &matrix, float x, float y) const
      {
        float x2 = x + batch.texture.innerWidth;
        float y2 = y + batch.texture.innerHeight;

        float u1 = batch.texture.u1;
        float v1 = batch.texture.v1;
        float u2 = batch.texture.u2;
        float v2 = batch.texture.v2;

        matrix.addTransformedQuad<GL_TRIANGLES, GL_CW>(Quad<XYZ.UV.RGBA>(x, y, x2, y2, u1, v1, u2, v2, color), batch);
      }

      // ---

      template <>
      void Texture::fill<GL_CCW>(IndexedVertexBatch<XYZ.UV, GLushort> &batch, float x, float y) const
      {
        float x2 = x + batch.texture.innerWidth;
        float y2 = y + batch.texture.innerHeight;

        float u1 = batch.texture.u1;
        float v1 = batch.texture.v1;
        float u2 = batch.texture.u2;
        float v2 = batch.texture.v2;

        batch
          .addVertex( x,  y, 0, u1, v1)
          .addVertex( x, y2, 0, u1, v2)
          .addVertex(x2, y2, 0, u2, v2)
          .addVertex(x2,  y, 0, u2, v1);

        batch
          .addIndices(0, 1, 2, 2, 3, 0)
          .incrementIndices(4);
      }

      template <>
      void Texture::fill<GL_CW>(IndexedVertexBatch<XYZ.UV, GLushort> &batch, float x, float y) const
      {
        float x2 = x + batch.texture.innerWidth;
        float y2 = y + batch.texture.innerHeight;

        float u1 = batch.texture.u1;
        float v1 = batch.texture.v1;
        float u2 = batch.texture.u2;
        float v2 = batch.texture.v2;

        batch
          .addVertex( x,  y, 0, u1, v1)
          .addVertex( x, y2, 0, u1, v2)
          .addVertex(x2, y2, 0, u2, v2)
          .addVertex(x2,  y, 0, u2, v1);

        batch
          .addIndices(0, 3, 2, 2, 1, 0)
          .incrementIndices(4);
      }

      template <>
      void Texture::fill<GL_CCW>(IndexedVertexBatch<XYZ.UV.RGBA, GLushort> &batch, float x, float y) const
      {
        float x2 = x + batch.texture.innerWidth;
        float y2 = y + batch.texture.innerHeight;

        float u1 = batch.texture.u1;
        float v1 = batch.texture.v1;
        float u2 = batch.texture.u2;
        float v2 = batch.texture.v2;

        batch
          .addVertex( x,  y, 0, u1, v1, color)
          .addVertex( x, y2, 0, u1, v2, color)
          .addVertex(x2, y2, 0, u2, v2, color)
          .addVertex(x2,  y, 0, u2, v1, color);

        batch
          .addIndices(0, 1, 2, 2, 3, 0)
          .incrementIndices(4);
      }

      template <>
      void Texture::fill<GL_CW>(IndexedVertexBatch<XYZ.UV.RGBA, GLushort> &batch, float x, float y) const
      {
        float x2 = x + batch.texture.innerWidth;
        float y2 = y + batch.texture.innerHeight;

        float u1 = batch.texture.u1;
        float v1 = batch.texture.v1;
        float u2 = batch.texture.u2;
        float v2 = batch.texture.v2;

        batch
          .addVertex( x,  y, 0, u1, v1, color)
          .addVertex( x, y2, 0, u1, v2, color)
          .addVertex(x2, y2, 0, u2, v2, color)
          .addVertex(x2,  y, 0, u2, v1, color);

        batch
          .addIndices(0, 3, 2, 2, 1, 0)
          .incrementIndices(4);
      }

      // ---

      template <>
      void Texture::fillRect<GL_CCW>(IndexedVertexBatch<XYZ.UV, GLushort> &batch, Matrix &matrix, const Rectf &rect, float ox, float oy)
      {
        float width = batch.texture.innerWidth;
        float height = batch.texture.innerHeight;

        float u1 = (rect.x1 - ox) / width;
        float v1 = (rect.y1 - oy) / height;
        float u2 = (rect.x2 - ox) / width;
        float v2 = (rect.y2 - oy) / height;

        matrix.addTransformedQuad<GL_TRIANGLES, GL_CCW>(Quad<XYZ.UV>(rect, u1, v1, u2, v2), batch);
      }

      template <>
      void Texture::fillRect<GL_CCW>(IndexedVertexBatch<XYZ.UV.RGBA, GLushort> &batch, Matrix &matrix, const Rectf &rect, float ox, float oy)
      {
        float width = batch.texture.innerWidth;
        float height = batch.texture.innerHeight;

        float u1 = (rect.x1 - ox) / width;
        float v1 = (rect.y1 - oy) / height;
        float u2 = (rect.x2 - ox) / width;
        float v2 = (rect.y2 - oy) / height;

        matrix.addTransformedQuad<GL_TRIANGLES, GL_CCW>(Quad<XYZ.UV.RGBA>(rect, u1, v1, u2, v2, color), batch);
      }
    }
  }
}
