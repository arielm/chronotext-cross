#include "chr/gl/draw/Sprite.h"

using namespace std;
using namespace chr;
using namespace math;

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

      Sprite& Sprite::setTextureBounds(const math::Rectf &bounds)
      {
        textureBounds = bounds;
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

      // ---

      template <>
      void Sprite::append(IndexedVertexBatch<XYZ.UV, GLuint> &batch, const Matrix &matrix, float x, float y) const
      {
        Rectf bounds;
        glm::vec2 coords1, coords2;
        tie(bounds, coords1, coords2) = getTextureQuad(batch->texture, x, y);

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
      void Sprite::append(IndexedVertexBatch<XYZ.UV.RGBA, GLuint> &batch, const Matrix &matrix, float x, float y) const
      {
        Rectf bounds;
        glm::vec2 coords1, coords2;
        tie(bounds, coords1, coords2) = getTextureQuad(batch->texture, x, y);

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
      void Sprite::append(IndexedVertexBatch<XYZ.UV, GLuint> &batch, float x, float y) const
      {
        Rectf bounds;
        glm::vec2 coords1, coords2;
        tie(bounds, coords1, coords2) = getTextureQuad(batch->texture, x, y);

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
      void Sprite::append(IndexedVertexBatch<XYZ.UV.RGBA, GLuint> &batch, float x, float y) const
      {
        Rectf bounds;
        glm::vec2 coords1, coords2;
        tie(bounds, coords1, coords2) = getTextureQuad(batch->texture, x, y);

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

      tuple<Rectf, glm::vec2, glm::vec2> Sprite::getTextureQuad(const Texture &texture, float x, float y) const
      {
        float width, height;
        glm::vec2 coords1, coords2;

        if (textureBounds.isNull())
        {
          width = texture.innerWidth;
          height = texture.innerHeight;

          coords1 = texture.coords1;
          coords2 = texture.coords2;
        }
        else
        {
          width = textureBounds.width();
          height = textureBounds.height();

          coords1 = textureBounds.x1y1() / texture.size;
          coords2 = textureBounds.x2y2() / texture.size;
        }

        Rectf bounds(x - width * anchor.x, y - height * anchor.y, width, height);

        return make_tuple(bounds, coords1, coords2);
      }
    }
  }
}
