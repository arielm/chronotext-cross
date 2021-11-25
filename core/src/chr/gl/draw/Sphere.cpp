#include "Sphere.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      Sphere& Sphere::setFrontFace(GLenum mode)
      {
        frontFace = mode;
        return *this;
      }

      Sphere& Sphere::setColor(const glm::vec4 &color)
      {
        this->color = color;
        return *this;
      }

      Sphere& Sphere::setColor(float r, float g, float b, float a)
      {
        color = { r, g, b, a };
        return *this;
      }

      Sphere& Sphere::setSectorCount(int count)
      {
        sectorCount = count;
        return *this;
      }

      Sphere& Sphere::setStackCount(int count)
      {
        stackCount = count;
        return *this;
      }

      Sphere& Sphere::setRadius(float radius)
      {
        this->radius = radius;
        return *this;
      }

      template <>
      void Sphere::append(IndexedVertexBatch<XYZ.N, GLuint> &batch, const Matrix &matrix) const
      {
        auto normalMatrix = matrix.getNormalMatrix();
        auto initialVertexCount = batch.vertexCount();
        float sectorStep = TWO_PI / sectorCount;
        float stackStep = PI / stackCount;

        for (int i = 0; i <= stackCount; ++i)
        {
          float stackAngle = HALF_PI - i * stackStep;
          float xy = radius * cosf(stackAngle);
          float z = radius * sinf(stackAngle);

          for (int j = 0; j <= sectorCount; ++j)
          {
            float sectorAngle = j * sectorStep;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;

            batch.addVertex(matrix.transformPoint(x, y, z), normalMatrix * glm::vec3(nx, ny, nz));
          }
        }

        for (int i = 0; i < stackCount; ++i)
        {
          float k1 = i * (sectorCount + 1);
          float k2 = k1 + sectorCount + 1;

          for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
          {
            if (i != 0)
            {
              if (frontFace == CCW)
              {
                batch.addIndices(k1, k1 + 1, k2);
              }
              else
              {
                batch.addIndices(k1, k2, k1 + 1);
              }
            }

            if (i != (stackCount-1))
            {
              if (frontFace == CCW)
              {
                batch.addIndices(k1 + 1, k2 + 1, k2);
              }
              else
              {
                batch.addIndices(k1 + 1, k2, k2 + 1);
              }
            }
          }
        }

        batch.incrementIndices(batch.vertexCount() - initialVertexCount);
      }

      template <>
      void Sphere::append(IndexedVertexBatch<XYZ.N.UV, GLuint> &batch, const Matrix &matrix) const
      {
        auto normalMatrix = matrix.getNormalMatrix();
        auto initialVertexCount = batch.vertexCount();
        float sectorStep = TWO_PI / sectorCount;
        float stackStep = PI / stackCount;

        for (int i = 0; i <= stackCount; ++i)
        {
          float stackAngle = HALF_PI - i * stackStep;
          float xy = radius * cosf(stackAngle);
          float z = radius * sinf(stackAngle);

          for (int j = 0; j <= sectorCount; ++j)
          {
            float sectorAngle = j * sectorStep;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;

            float s = (float)j / sectorCount;
            float t = (float)i / stackCount;

            batch.addVertex(matrix.transformPoint(x, y, z), normalMatrix * glm::vec3(nx, ny, nz), glm::vec2(s, t));
          }
        }

        for (int i = 0; i < stackCount; ++i)
        {
          float k1 = i * (sectorCount + 1);
          float k2 = k1 + sectorCount + 1;

          for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
          {
            if (i != 0)
            {
              if (frontFace == CCW)
              {
                batch.addIndices(k1, k1 + 1, k2);
              }
              else
              {
                batch.addIndices(k1, k2, k1 + 1);
              }
            }

            if (i != (stackCount-1))
            {
              if (frontFace == CCW)
              {
                batch.addIndices(k1 + 1, k2 + 1, k2);
              }
              else
              {
                batch.addIndices(k1 + 1, k2, k2 + 1);
              }
            }
          }
        }

        batch.incrementIndices(batch.vertexCount() - initialVertexCount);
      }

      template <>
      void Sphere::append(IndexedVertexBatch<XYZ.N.RGBA, GLuint> &batch, const Matrix &matrix) const
      {
        auto normalMatrix = matrix.getNormalMatrix();
        auto initialVertexCount = batch.vertexCount();
        float sectorStep = TWO_PI / sectorCount;
        float stackStep = PI / stackCount;

        for (int i = 0; i <= stackCount; ++i)
        {
          float stackAngle = HALF_PI - i * stackStep;
          float xy = radius * cosf(stackAngle);
          float z = radius * sinf(stackAngle);

          for (int j = 0; j <= sectorCount; ++j)
          {
            float sectorAngle = j * sectorStep;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;

            batch.addVertex(matrix.transformPoint(x, y, z), normalMatrix * glm::vec3(nx, ny, nz), color);
          }
        }

        for (int i = 0; i < stackCount; ++i)
        {
          float k1 = i * (sectorCount + 1);
          float k2 = k1 + sectorCount + 1;

          for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
          {
            if (i != 0)
            {
              if (frontFace == CCW)
              {
                batch.addIndices(k1, k1 + 1, k2);
              }
              else
              {
                batch.addIndices(k1, k2, k1 + 1);
              }
            }

            if (i != (stackCount-1))
            {
              if (frontFace == CCW)
              {
                batch.addIndices(k1 + 1, k2 + 1, k2);
              }
              else
              {
                batch.addIndices(k1 + 1, k2, k2 + 1);
              }
            }
          }
        }

        batch.incrementIndices(batch.vertexCount() - initialVertexCount);
      }

      template <>
      void Sphere::append(IndexedVertexBatch<XYZ.N.UV.RGBA, GLuint> &batch, const Matrix &matrix) const
      {
        auto normalMatrix = matrix.getNormalMatrix();
        auto initialVertexCount = batch.vertexCount();
        float sectorStep = TWO_PI / sectorCount;
        float stackStep = PI / stackCount;

        for (int i = 0; i <= stackCount; ++i)
        {
          float stackAngle = HALF_PI - i * stackStep;
          float xy = radius * cosf(stackAngle);
          float z = radius * sinf(stackAngle);

          for (int j = 0; j <= sectorCount; ++j)
          {
            float sectorAngle = j * sectorStep;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;

            float s = (float)j / sectorCount;
            float t = (float)i / stackCount;

            batch.addVertex(matrix.transformPoint(x, y, z), normalMatrix * glm::vec3(nx, ny, nz), glm::vec2(s, t), color);
          }
        }

        for (int i = 0; i < stackCount; ++i)
        {
          float k1 = i * (sectorCount + 1);
          float k2 = k1 + sectorCount + 1;

          for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
          {
            if (i != 0)
            {
              if (frontFace == CCW)
              {
                batch.addIndices(k1, k1 + 1, k2);
              }
              else
              {
                batch.addIndices(k1, k2, k1 + 1);
              }
            }

            if (i != (stackCount-1))
            {
              if (frontFace == CCW)
              {
                batch.addIndices(k1 + 1, k2 + 1, k2);
              }
              else
              {
                batch.addIndices(k1 + 1, k2, k2 + 1);
              }
            }
          }
        }

        batch.incrementIndices(batch.vertexCount() - initialVertexCount);
      }
    }
  }
}
