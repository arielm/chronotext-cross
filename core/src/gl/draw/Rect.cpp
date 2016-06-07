#include "gl/draw/Rect.h"

using namespace std;
using namespace chr::math;

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      Rect& Rect::setFrontFace(GLenum mode)
      {
        frontFace = mode;
        return *this;
      }

      Rect& Rect::setTextureOffset(const glm::vec2 &offset)
      {
        textureOffset = offset;
        return *this;
      }

      Rect& Rect::setTextureOffset(float x, float y)
      {
        textureOffset = glm::vec2(x, y);
        return *this;
      }

      Rect& Rect::setTextureScale(float scale)
      {
        textureScale = scale;
        return *this;
      }

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

      Rect& Rect::setBounds(const math::Rectf &bounds)
      {
        this->bounds = bounds;
        return *this;
      }

      Rect& Rect::setBounds(const glm::vec2 &upperLeft, const glm::vec2 &lowerRight)
      {
        bounds = Rectf(upperLeft, lowerRight);
        return *this;
      }

      Rect& Rect::setBounds(float left, float top, float width, float height)
      {
        bounds = Rectf(left, top, width, height);
        return *this;
      }

      // ---

      template <>
      void Rect::append(IndexedVertexBatch<XYZ, GLushort> &batch, Matrix &matrix) const
      {
        if (frontFace == CW)
        {
          matrix.addTransformedQuad<GL_TRIANGLES, CW>(Quad<XYZ>(bounds), batch);
        }
        else
        {
          matrix.addTransformedQuad<GL_TRIANGLES, CCW>(Quad<XYZ>(bounds), batch);
        }
      }

      template <>
      void Rect::append(IndexedVertexBatch<XYZ.UV, GLushort> &batch, Matrix &matrix) const
      {
        glm::vec2 coords1, coords2;
        tie(coords1, coords2) = getTextureCoords(batch.texture);

        if (frontFace == CW)
        {
          matrix.addTransformedQuad<GL_TRIANGLES, CW>(Quad<XYZ.UV>(bounds, coords1, coords2), batch);
        }
        else
        {
          matrix.addTransformedQuad<GL_TRIANGLES, CCW>(Quad<XYZ.UV>(bounds, coords1, coords2), batch);
        }
      }

      template <>
      void Rect::append(IndexedVertexBatch<XYZ.RGBA, GLushort> &batch, Matrix &matrix) const
      {
        if (frontFace == CW)
        {
          matrix.addTransformedQuad<GL_TRIANGLES, CW>(Quad<XYZ.RGBA>(bounds, color), batch);
        }
        else
        {
          matrix.addTransformedQuad<GL_TRIANGLES, CCW>(Quad<XYZ.RGBA>(bounds, color), batch);
        }
      }

      template <>
      void Rect::append(IndexedVertexBatch<XYZ.N.RGBA, GLushort> &batch, Matrix &matrix) const
      {
        if (frontFace == CW)
        {
          matrix.addTransformedQuad<GL_TRIANGLES, CW>(Quad<XYZ.N.RGBA>(bounds, glm::vec3(0, 0, +1), color), batch);
        }
        else
        {
          matrix.addTransformedQuad<GL_TRIANGLES, CCW>(Quad<XYZ.N.RGBA>(bounds, glm::vec3(0, 0, +1), color), batch);
        }
      }

      template <>
      void Rect::append(IndexedVertexBatch<XYZ.UV.RGBA, GLushort> &batch, Matrix &matrix) const
      {
        glm::vec2 coords1, coords2;
        tie(coords1, coords2) = getTextureCoords(batch.texture);

        if (frontFace == CW)
        {
          matrix.addTransformedQuad<GL_TRIANGLES, CW>(Quad<XYZ.UV.RGBA>(bounds, coords1, coords2, color), batch);
        }
        else
        {
          matrix.addTransformedQuad<GL_TRIANGLES, CCW>(Quad<XYZ.UV.RGBA>(bounds, coords1, coords2, color), batch);
        }
      }

      // ---

      template <>
      void Rect::append(IndexedVertexBatch<XYZ, GLushort> &batch) const
      {
        batch
          .addVertex(bounds.x1, bounds.y1)
          .addVertex(bounds.x1, bounds.y2)
          .addVertex(bounds.x2, bounds.y2)
          .addVertex(bounds.x2, bounds.y1);

        if (frontFace == CW)
        {
          batch.addIndices(0, 3, 2, 2, 1, 0);
        }
        else
        {
          batch.addIndices(0, 1, 2, 2, 3, 0);
        }

        batch.incrementIndices(4);
      }

      template <>
      void Rect::append(IndexedVertexBatch<XYZ.UV, GLushort> &batch) const
      {
        glm::vec2 coords1, coords2;
        tie(coords1, coords2) = getTextureCoords(batch.texture);

        batch
          .addVertex(bounds.x1, bounds.y1, 0, coords1.x, coords1.y)
          .addVertex(bounds.x1, bounds.y2, 0, coords1.x, coords2.y)
          .addVertex(bounds.x2, bounds.y2, 0, coords2.x, coords2.y)
          .addVertex(bounds.x2, bounds.y1, 0, coords2.x, coords1.y);

        if (frontFace == CW)
        {
          batch.addIndices(0, 3, 2, 2, 1, 0);
        }
        else
        {
          batch.addIndices(0, 1, 2, 2, 3, 0);
        }

        batch.incrementIndices(4);
      }

      template <>
      void Rect::append(IndexedVertexBatch<XYZ.RGBA, GLushort> &batch) const
      {
        batch
          .addVertex(bounds.x1, bounds.y1, 0, color)
          .addVertex(bounds.x1, bounds.y2, 0, color)
          .addVertex(bounds.x2, bounds.y2, 0, color)
          .addVertex(bounds.x2, bounds.y1, 0, color);

        if (frontFace == CW)
        {
          batch.addIndices(0, 3, 2, 2, 1, 0);
        }
        else
        {
          batch.addIndices(0, 1, 2, 2, 3, 0);
        }

        batch.incrementIndices(4);
      }

      template <>
      void Rect::append(IndexedVertexBatch<XYZ.UV.RGBA, GLushort> &batch) const
      {
        glm::vec2 coords1, coords2;
        tie(coords1, coords2) = getTextureCoords(batch.texture);

        batch
          .addVertex(bounds.x1, bounds.y1, 0, coords1.x, coords1.y, color)
          .addVertex(bounds.x1, bounds.y2, 0, coords1.x, coords2.y, color)
          .addVertex(bounds.x2, bounds.y2, 0, coords2.x, coords2.y, color)
          .addVertex(bounds.x2, bounds.y1, 0, coords2.x, coords1.y, color);

        if (frontFace == CW)
        {
          batch.addIndices(0, 3, 2, 2, 1, 0);
        }
        else
        {
          batch.addIndices(0, 1, 2, 2, 3, 0);
        }

        batch.incrementIndices(4);
      }

      // ---

      pair<glm::vec2, glm::vec2> Rect::getTextureCoords(const Texture &texture) const
      {
        auto scaledSize = texture.innerSize * textureScale;

        glm::vec2 coords1 = (bounds.x1y1() - textureOffset) / scaledSize;
        glm::vec2 coords2 = (bounds.x2y2() - textureOffset) / scaledSize;

        return make_pair(coords1, coords2);
      }
    }
  }
}
