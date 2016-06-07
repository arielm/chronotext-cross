#include "gl/draw/Circle.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      Circle& Circle::setFrontFace(GLenum mode)
      {
        frontFace = mode;
        return *this;
      }

      Circle& Circle::setTextureOffset(const glm::vec2 &offset)
      {
        textureOffset = offset;
        return *this;
      }

      Circle& Circle::setTextureOffset(float x, float y)
      {
        textureOffset = glm::vec2(x, y);
        return *this;
      }

      Circle& Circle::setTextureScale(float scale)
      {
        textureScale = scale;
        return *this;
      }

      Circle& Circle::setColor(const glm::vec4 &color)
      {
        this->color = color;
        return *this;
      }

      Circle& Circle::setColor(float r, float g, float b, float a)
      {
        color = { r, g, b, a };
        return *this;
      }

      Circle& Circle::setRadius(float radius)
      {
        r = radius;
        return *this;
      }

      Circle& Circle::setArc(float a1, float a2)
      {
        this->a1 = a1;
        this->a2 = a2;
        return *this;
      }

      Circle& Circle::setSegmentLength(float length)
      {
        segmentLength = length;
        return *this;
      }

      // ---

      template <>
      void Circle::append(IndexedVertexBatch<XYZ, GLushort> &batch, Matrix &matrix, float x, float y) const
      {
        float aa = fabsf(a2 - a1);
        int n = ceilf(aa * r / segmentLength) + 1;

        batch.addVertex(matrix.transformPoint(x, y));

        for (int i = 0; i < n; i++)
        {
          float dd = a1 + fminf(aa, i * segmentLength / r);
          float xx = +sinf(dd) * r;
          float yy = +cosf(dd) * r;

          batch.addVertex(matrix.transformPoint(x + xx, y + yy));

          if (i < n - 1)
          {
            if (frontFace == CW)
            {
              batch.addIndices(i + 1, 0, i + 2);
            }
            else
            {
              batch.addIndices(0, i + 1, i + 2);
            }
          }
        }

        batch.incrementIndices(n + 1);
      }

      template <>
      void Circle::append(IndexedVertexBatch<XYZ.UV, GLushort> &batch, Matrix &matrix, float x, float y) const
      {
        float aa = fabsf(a2 - a1);
        int n = ceilf(aa * r / segmentLength) + 1;

        batch.addVertex(matrix.transformPoint(x, y), getTextureCoords(batch.texture, 0, 0));

        for (int i = 0; i < n; i++)
        {
          float dd = a1 + fminf(aa, i * segmentLength / r);
          float xx = +sinf(dd) * r;
          float yy = +cosf(dd) * r;

          batch.addVertex(matrix.transformPoint(x + xx, y + yy), getTextureCoords(batch.texture, xx, yy));

          if (i < n - 1)
          {
            if (frontFace == CW)
            {
              batch.addIndices(i + 1, 0, i + 2);
            }
            else
            {
              batch.addIndices(0, i + 1, i + 2);
            }
          }
        }

        batch.incrementIndices(n + 1);
      }

      template <>
      void Circle::append(IndexedVertexBatch<XYZ.RGBA, GLushort> &batch, Matrix &matrix, float x, float y) const
      {
        float aa = fabsf(a2 - a1);
        int n = ceilf(aa * r / segmentLength) + 1;

        batch.addVertex(matrix.transformPoint(x, y), color);

        for (int i = 0; i < n; i++)
        {
          float dd = a1 + fminf(aa, i * segmentLength / r);
          float xx = +sinf(dd) * r;
          float yy = +cosf(dd) * r;

          batch.addVertex(matrix.transformPoint(x + xx, y + yy), color);

          if (i < n - 1)
          {
            if (frontFace == CW)
            {
              batch.addIndices(i + 1, 0, i + 2);
            }
            else
            {
              batch.addIndices(0, i + 1, i + 2);
            }
          }
        }

        batch.incrementIndices(n + 1);
      }

      template <>
      void Circle::append(IndexedVertexBatch<XYZ.UV.RGBA, GLushort> &batch, Matrix &matrix, float x, float y) const
      {
        float aa = fabsf(a2 - a1);
        int n = ceilf(aa * r / segmentLength) + 1;

        batch.addVertex(matrix.transformPoint(x, y), getTextureCoords(batch.texture, 0, 0), color);

        for (int i = 0; i < n; i++)
        {
          float dd = a1 + fminf(aa, i * segmentLength / r);
          float xx = +sinf(dd) * r;
          float yy = +cosf(dd) * r;

          batch.addVertex(matrix.transformPoint(x + xx, y + yy), getTextureCoords(batch.texture, xx, yy), color);

          if (i < n - 1)
          {
            if (frontFace == CW)
            {
              batch.addIndices(i + 1, 0, i + 2);
            }
            else
            {
              batch.addIndices(0, i + 1, i + 2);
            }
          }
        }

        batch.incrementIndices(n + 1);
      }

      // ---

      glm::vec2 Circle::getTextureCoords(const Texture &texture, float x, float y) const
      {
        return (glm::vec2(x, y) - textureOffset) / (texture.innerSize * textureScale);
      }
    }
  }
}
