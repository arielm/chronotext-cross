#include "gl/draw/Sprite.h"

using namespace std;
using namespace chr::math;

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      Sprite& Sprite::setFrontFace(GLenum mode)
      {
        frontFace = mode;
        return *this;
      }

      Sprite& Sprite::setAnchor(const glm::vec2 &anchor)
      {
        this->anchor = anchor;
        return *this;
      }

      Sprite& Sprite::setAnchor(float x, float y)
      {
        anchor = glm::vec2(x, y);
        return *this;
      }

      Sprite& Sprite::setColor(const glm::vec4 &color)
      {
        this->color = color;
        return *this;
      }

      Sprite& Sprite::setColor(float r, float g, float b, float a)
      {
        color = { r, g, b, a };
        return *this;
      }

      // ---

      template <>
      void Sprite::append(IndexedVertexBatch<XYZ.UV, GLushort> &batch, Matrix &matrix, float x, float y) const
      {
        Rectf rect;
        glm::vec2 coords1, coords2;
        tie(rect, coords1, coords2) = getTexturedQuad(batch.texture, x, y);

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
      void Sprite::append(IndexedVertexBatch<XYZ.UV.RGBA, GLushort> &batch, Matrix &matrix, float x, float y) const
      {
        Rectf rect;
        glm::vec2 coords1, coords2;
        tie(rect, coords1, coords2) = getTexturedQuad(batch.texture, x, y);

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
      void Sprite::append(IndexedVertexBatch<XYZ.UV, GLushort> &batch, float x, float y) const
      {
        Rectf rect;
        glm::vec2 coords1, coords2;
        tie(rect, coords1, coords2) = getTexturedQuad(batch.texture, x, y);

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
      void Sprite::append(IndexedVertexBatch<XYZ.UV.RGBA, GLushort> &batch, float x, float y) const
      {
        Rectf rect;
        glm::vec2 coords1, coords2;
        tie(rect, coords1, coords2) = getTexturedQuad(batch.texture, x, y);

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

      tuple<Rectf, glm::vec2, glm::vec2> Sprite::getTexturedQuad(const Texture &texture, float x, float y) const
      {
        float width = texture.innerWidth;
        float height = texture.innerHeight;
        Rectf rect = Rectf(x - width * anchor.x, y - height * anchor.y, width, height);

        glm::vec2 coords1 = texture.coords1;
        glm::vec2  coords2 = texture.coords2;

        return make_tuple(rect, coords1, coords2);
      }
    }
  }
}
