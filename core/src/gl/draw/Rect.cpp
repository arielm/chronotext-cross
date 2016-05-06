#include "gl/draw/Rect.h"

using namespace chr::math;

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      Rect& Rect::setColor(const glm::vec4 &color)
      {
        this->color = color;
        return *this;
      }

      Rect& Rect::setColor(float r, float g, float b, float a)
      {
        color = { r, g, b, a };
        return *this;
      }

      template <>
      void Rect::fill<GL_CCW>(IndexedVertexBatch<XYZ, GLushort> &batch, Matrix &matrix, float x1, float y1, float x2, float y2) const
      {
        matrix.addTransformedQuad<GL_TRIANGLES, GL_CCW>(Quad<XYZ>(x1, y1, x2, y2), batch);
      }

      template <>
      void Rect::fill<GL_CW>(IndexedVertexBatch<XYZ, GLushort> &batch, Matrix &matrix, float x1, float y1, float x2, float y2) const
      {
        matrix.addTransformedQuad<GL_TRIANGLES, GL_CW>(Quad<XYZ>(x1, y1, x2, y2), batch);
      }

      template <>
      void Rect::fill<GL_CCW>(IndexedVertexBatch<XYZ.RGBA, GLushort> &batch, Matrix &matrix, float x1, float y1, float x2, float y2) const
      {
        matrix.addTransformedQuad<GL_TRIANGLES, GL_CCW>(Quad<XYZ.RGBA>(x1, y1, x2, y2, color), batch);
      }

      template <>
      void Rect::fill<GL_CW>(IndexedVertexBatch<XYZ.RGBA, GLushort> &batch, Matrix &matrix, float x1, float y1, float x2, float y2) const
      {
        matrix.addTransformedQuad<GL_TRIANGLES, GL_CW>(Quad<XYZ.RGBA>(x1, y1, x2, y2, color), batch);
      }

      template <>
      void Rect::fill<GL_CCW>(IndexedVertexBatch<XYZ.N.RGBA, GLushort> &batch, Matrix &matrix, float x1, float y1, float x2, float y2) const
      {
        matrix.addTransformedQuad<GL_TRIANGLES, GL_CCW>(Quad<XYZ.N.RGBA>(x1, y1, x2, y2, glm::vec3(0, 0, +1), color), batch);
      }

      template <>
      void Rect::fill<GL_CW>(IndexedVertexBatch<XYZ.N.RGBA, GLushort> &batch, Matrix &matrix, float x1, float y1, float x2, float y2) const
      {
        matrix.addTransformedQuad<GL_TRIANGLES, GL_CW>(Quad<XYZ.N.RGBA>(x1, y1, x2, y2, glm::vec3(0, 0, -1), color), batch);
      }

      // ---

      template <>
      void Rect::fill<GL_CCW>(IndexedVertexBatch<XYZ, GLushort> &batch, float x1, float y1, float x2, float y2) const
      {
        batch
          .addVertex(x1, y1)
          .addVertex(x1, y2)
          .addVertex(x2, y2)
          .addVertex(x2, y1);

        batch
          .addIndices(0, 1, 2, 2, 3, 0)
          .incrementIndices(4);
      }

      template <>
      void Rect::fill<GL_CW>(IndexedVertexBatch<XYZ, GLushort> &batch, float x1, float y1, float x2, float y2) const
      {
        batch
          .addVertex(x1, y1)
          .addVertex(x1, y2)
          .addVertex(x2, y2)
          .addVertex(x2, y1);

        batch
          .addIndices(0, 3, 2, 2, 1, 0)
          .incrementIndices(4);
      }

      template <>
      void Rect::fill<GL_CCW>(IndexedVertexBatch<XYZ.RGBA, GLushort> &batch, float x1, float y1, float x2, float y2) const
      {
        batch
          .addVertex(x1, y1, 0, color)
          .addVertex(x1, y2, 0, color)
          .addVertex(x2, y2, 0, color)
          .addVertex(x2, y1, 0, color);

        batch
          .addIndices(0, 1, 2, 2, 3, 0)
          .incrementIndices(4);
      }

      template <>
      void Rect::fill<GL_CW>(IndexedVertexBatch<XYZ.RGBA, GLushort> &batch, float x1, float y1, float x2, float y2) const
      {
        batch
          .addVertex(x1, y1, 0, color)
          .addVertex(x1, y2, 0, color)
          .addVertex(x2, y2, 0, color)
          .addVertex(x2, y1, 0, color);

        batch
          .addIndices(0, 3, 2, 2, 1, 0)
          .incrementIndices(4);
      }
    }
  }
}
