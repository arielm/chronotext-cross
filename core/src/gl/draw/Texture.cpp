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
        float u1, v1, u2, v2;
        tie(rect, u1, v1, u2, v2) = computeTexturedQuad(batch.texture, x, y);

        if (frontFace == GL_CW)
        {
          matrix.addTransformedQuad<GL_TRIANGLES, GL_CW>(Quad<XYZ.UV>(rect, u1, v1, u2, v2), batch);
        }
        else
        {
          matrix.addTransformedQuad<GL_TRIANGLES, GL_CCW>(Quad<XYZ.UV>(rect, u1, v1, u2, v2), batch);
        }
      }

      template <>
      void Texture::append(IndexedVertexBatch<XYZ.UV.RGBA, GLushort> &batch, Matrix &matrix, float x, float y) const
      {
        Rectf rect;
        float u1, v1, u2, v2;
        tie(rect, u1, v1, u2, v2) = computeTexturedQuad(batch.texture, x, y);

        if (frontFace == GL_CW)
        {
          matrix.addTransformedQuad<GL_TRIANGLES, GL_CW>(Quad<XYZ.UV.RGBA>(rect, u1, v1, u2, v2, color), batch);
        }
        else
        {
          matrix.addTransformedQuad<GL_TRIANGLES, GL_CCW>(Quad<XYZ.UV.RGBA>(rect, u1, v1, u2, v2, color), batch);
        }
      }

      // ---

      template <>
      void Texture::append(IndexedVertexBatch<XYZ.UV, GLushort> &batch, float x, float y) const
      {
        Rectf rect;
        float u1, v1, u2, v2;
        tie(rect, u1, v1, u2, v2) = computeTexturedQuad(batch.texture, x, y);

        batch
          .addVertex(rect.x1, rect.y1, 0, u1, v1)
          .addVertex(rect.x1, rect.y2, 0, u1, v2)
          .addVertex(rect.x2, rect.y2, 0, u2, v2)
          .addVertex(rect.x2, rect.y1, 0, u2, v1);

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
        float u1, v1, u2, v2;
        tie(rect, u1, v1, u2, v2) = computeTexturedQuad(batch.texture, x, y);

        batch
          .addVertex(rect.x1, rect.y1, 0, u1, v1, color)
          .addVertex(rect.x1, rect.y2, 0, u1, v2, color)
          .addVertex(rect.x2, rect.y2, 0, u2, v2, color)
          .addVertex(rect.x2, rect.y1, 0, u2, v1, color);

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

      tuple<Rectf, float, float, float, float> Texture::computeTexturedQuad(const gl::Texture &texture, float x, float y) const
      {
        Rectf rect;
        float u1, v1, u2, v2;

        float width = texture.innerWidth;
        float height = texture.innerHeight;

        if (hasBounds)
        {
          rect = bounds;

          u1 = (rect.x1 - offset.x) / width;
          v1 = (rect.y1 - offset.y) / height;
          u2 = (rect.x2 - offset.x) / width;
          v2 = (rect.y2 - offset.y) / height;
        }
        else
        {
          rect = Rectf(x - width * anchor.x, y - height * anchor.y, width, height);

          u1 = texture.u1;
          v1 = texture.v1;
          u2 = texture.u2;
          v2 = texture.v2;
        }

        return make_tuple(rect, u1, v1, u2, v2);
      }
    }
  }
}
