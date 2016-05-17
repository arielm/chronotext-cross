#include "gl/draw/Texture.h"

using namespace std;
using namespace chr::math;

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      Texture& Texture::setFrontFace(GLenum mode)
      {
        frontFace = mode;
        return *this;
      }

      Texture& Texture::setBounds(const math::Rectf &bounds)
      {
        this->bounds = bounds;
        hasBounds = true;

        return *this;
      }

      Texture& Texture::setBounds(const glm::vec2 &upperLeft, const glm::vec2 &lowerRight)
      {
        bounds = Rectf(upperLeft, lowerRight);
        hasBounds = true;

        return *this;
      }

      Texture& Texture::setBounds(float left, float top, float width, float height)
      {
        bounds = Rectf(left, top, width, height);
        hasBounds = true;

        return *this;
      }

      Texture& Texture::setAnchor(const glm::vec2 &anchor)
      {
        this->anchor = anchor;
        return *this;
      }

      Texture& Texture::setAnchor(float x, float y)
      {
        anchor = glm::vec2(x, y);
        return *this;
      }

      Texture& Texture::setOffset(const glm::vec2 &offset)
      {
        this->offset = anchor;
        return *this;
      }

      Texture& Texture::setOffset(float x, float y)
      {
        offset = glm::vec2(x, y);
        return *this;
      }

      Texture& Texture::setScale(float scale)
      {
        this->scale = scale;
        return *this;
      }

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
      void Texture::append(IndexedVertexBatch<XYZ.UV, GLushort> &batch, Matrix &matrix, float x, float y) const
      {
        Rectf rect;
        glm::vec2 coords1, coords2;
        tie(rect, coords1, coords2) = computeTexturedQuad(batch.texture, x, y);

        if (frontFace == GL_CW)
        {
          matrix.addTransformedQuad<GL_TRIANGLES, GL_CW>(Quad<XYZ.UV>(rect, coords1, coords2), batch);
        }
        else
        {
          matrix.addTransformedQuad<GL_TRIANGLES, GL_CCW>(Quad<XYZ.UV>(rect, coords1, coords2), batch);
        }
      }

      template <>
      void Texture::append(IndexedVertexBatch<XYZ.UV.RGBA, GLushort> &batch, Matrix &matrix, float x, float y) const
      {
        Rectf rect;
        glm::vec2 coords1, coords2;
        tie(rect, coords1, coords2) = computeTexturedQuad(batch.texture, x, y);

        if (frontFace == GL_CW)
        {
          matrix.addTransformedQuad<GL_TRIANGLES, GL_CW>(Quad<XYZ.UV.RGBA>(rect, coords1, coords2, color), batch);
        }
        else
        {
          matrix.addTransformedQuad<GL_TRIANGLES, GL_CCW>(Quad<XYZ.UV.RGBA>(rect, coords1, coords2, color), batch);
        }
      }

      // ---

      template <>
      void Texture::append(IndexedVertexBatch<XYZ.UV, GLushort> &batch, float x, float y) const
      {
        Rectf rect;
        glm::vec2 coords1, coords2;
        tie(rect, coords1, coords2) = computeTexturedQuad(batch.texture, x, y);

        batch
          .addVertex(rect.x1, rect.y1, 0, coords1.x, coords1.y)
          .addVertex(rect.x1, rect.y2, 0, coords1.x, coords2.y)
          .addVertex(rect.x2, rect.y2, 0, coords2.x, coords2.y)
          .addVertex(rect.x2, rect.y1, 0, coords2.x, coords1.y);

        if (frontFace == GL_CW)
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
      void Texture::append(IndexedVertexBatch<XYZ.UV.RGBA, GLushort> &batch, float x, float y) const
      {
        Rectf rect;
        glm::vec2 coords1, coords2;
        tie(rect, coords1, coords2) = computeTexturedQuad(batch.texture, x, y);

        batch
          .addVertex(rect.x1, rect.y1, 0, coords1.x, coords1.y, color)
          .addVertex(rect.x1, rect.y2, 0, coords1.x, coords2.y, color)
          .addVertex(rect.x2, rect.y2, 0, coords2.x, coords2.y, color)
          .addVertex(rect.x2, rect.y1, 0, coords2.x, coords1.y, color);

        if (frontFace == GL_CW)
        {
          batch.addIndices(0, 3, 2, 2, 1, 0);
        }
        else
        {
          batch.addIndices(0, 1, 2, 2, 3, 0);
        }

        batch.incrementIndices(4);
      }

      tuple<Rectf, glm::vec2, glm::vec2> Texture::computeTexturedQuad(const gl::Texture &texture, float x, float y) const
      {
        Rectf rect;
        glm::vec2 coords1, coords2;

        if (hasBounds)
        {
          rect = bounds;
          auto scaledSize = texture.innerSize * scale;

          coords1 = (rect.x1y1() - offset) / scaledSize;
          coords2 = (rect.x2y2() - offset) / scaledSize;
        }
        else
        {
          float width = texture.innerWidth * scale;
          float height = texture.innerHeight * scale;
          rect = Rectf(x - width * anchor.x, y - height * anchor.y, width, height);

          coords1 = texture.coords1;
          coords2 = texture.coords2;
        }

        return make_tuple(rect, coords1, coords2);
      }
    }
  }
}
