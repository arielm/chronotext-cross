#include "Cylinder.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      Cylinder& Cylinder::setFrontFace(GLenum mode)
      {
        frontFace = mode;
        return *this;
      }

      Cylinder& Cylinder::setColor(const glm::vec4 &color)
      {
        this->color = color;
        return *this;
      }

      Cylinder& Cylinder::setColor(float r, float g, float b, float a)
      {
        color = { r, g, b, a };
        return *this;
      }

      Cylinder& Cylinder::setSectorCount(int count)
      {
        sectorCount = count;
        return *this;
      }

      Cylinder& Cylinder::setStackCount(int count)
      {
        stackCount = count;
        return *this;
      }

      Cylinder& Cylinder::setBaseRadius(float radius)
      {
        baseRadius = radius;
        return *this;
      }

      Cylinder& Cylinder::setTopRadius(float radius)
      {
        topRadius = radius;
        return *this;
      }

      Cylinder& Cylinder::setHeight(float height)
      {
        this->height = height;
        return *this;
      }

      template <>
      void Cylinder::append(IndexedVertexBatch<XYZ.N, GLuint> &batch, const Matrix &matrix) const
      {
        auto normalMatrix = matrix.getNormalMatrix();
        auto initialVertexCount = batch.vertexCount();
        auto unitCircleVertices = getUnitCircleVertices();
        auto sideNormals = getSideNormals();
        int positionIndex = 0;

        for (int i = 0; i <= stackCount; ++i)
        {
          float z = -(height * 0.5f) + (float)i / stackCount * height;
          float radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);

          for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
          {
            float x = unitCircleVertices[k];
            float y = unitCircleVertices[k+1];

            batch.addVertex(matrix.transformPoint(x * radius, y * radius, z), normalMatrix * glm::vec3(sideNormals[k], sideNormals[k+1], sideNormals[k+2]));
            positionIndex++;
          }
        }

        //

        int basePositionIndex = positionIndex;

        float z = -height * 0.5f;
        batch.addVertex(matrix.transformPoint(0, 0, z), normalMatrix * glm::vec3(0, 0, -1));
        positionIndex++;

        for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
        {
          float x = unitCircleVertices[j];
          float y = unitCircleVertices[j+1];

          batch.addVertex(matrix.transformPoint(x * baseRadius, y * baseRadius, z), normalMatrix * glm::vec3(0, 0, -1));
          positionIndex++;
        }

        //

        int topPositionIndex = positionIndex;

        z = height * 0.5f;
        batch.addVertex(matrix.transformPoint(0, 0, z), normalMatrix * glm::vec3(0, 0, 1));
        positionIndex++;

        for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
        {
          float x = unitCircleVertices[j];
          float y = unitCircleVertices[j+1];

          batch.addVertex(matrix.transformPoint(x * topRadius, y * topRadius, z), normalMatrix * glm::vec3(0, 0, 1));
          positionIndex++;
        }

        //

        for (int i = 0; i < stackCount; ++i)
        {
          int k1 = i * (sectorCount + 1);
          int k2 = k1 + sectorCount + 1;

          for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
          {
            if (frontFace == CCW)
            {
              batch.addIndices(k1, k2, k1 + 1, k2, k2 + 1, k1 + 1);
            }
            else
            {
              batch.addIndices(k1, k1 + 1, k2, k2, k1 + 1, k2 + 1);
            }
          }
        }

        for (int i = 0, k = basePositionIndex + 1; i < sectorCount; ++i, ++k)
        {
          if (i < (sectorCount - 1))
          {
            if (frontFace == CCW)
            {
              batch.addIndices(basePositionIndex, k, k + 1);
            }
            else
            {
              batch.addIndices(basePositionIndex, k + 1, k);
            }
          }
          else
          {
            if (frontFace == CCW)
            {
              batch.addIndices(basePositionIndex, k, basePositionIndex + 1);
            }
            else
            {
              batch.addIndices(basePositionIndex, basePositionIndex + 1, k);
            }
          }
        }

        for (int i = 0, k = topPositionIndex + 1; i < sectorCount; ++i, ++k)
        {
          if (i < (sectorCount - 1))
          {
            if (frontFace == CCW)
            {
              batch.addIndices(topPositionIndex, k + 1, k);
            }
            else
            {
              batch.addIndices(topPositionIndex, k, k + 1);
            }
          }
          else
          {
            if (frontFace == CCW)
            {
              batch.addIndices(topPositionIndex, topPositionIndex + 1, k);
            }
            else
            {
              batch.addIndices(topPositionIndex, k, topPositionIndex + 1);
            }
          }
        }

        batch.incrementIndices(batch.vertexCount() - initialVertexCount);
      }

      template <>
      void Cylinder::append(IndexedVertexBatch<XYZ.N.UV, GLuint> &batch, const Matrix &matrix) const
      {
        auto normalMatrix = matrix.getNormalMatrix();
        auto initialVertexCount = batch.vertexCount();
        auto unitCircleVertices = getUnitCircleVertices();
        auto sideNormals = getSideNormals();
        int positionIndex = 0;

        for (int i = 0; i <= stackCount; ++i)
        {
          float z = -(height * 0.5f) + (float)i / stackCount * height;
          float radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);
          float t = 1.0f - (float)i / stackCount;

          for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
          {
            float x = unitCircleVertices[k];
            float y = unitCircleVertices[k+1];

            batch.addVertex(matrix.transformPoint(x * radius, y * radius, z), normalMatrix * glm::vec3(sideNormals[k], sideNormals[k+1], sideNormals[k+2]), glm::vec2((float)j / sectorCount, t));
            positionIndex++;
          }
        }

        //

        int basePositionIndex = positionIndex;

        float z = -height * 0.5f;
        batch.addVertex(matrix.transformPoint(0, 0, z), normalMatrix * glm::vec3(0, 0, -1), glm::vec2(0.5f, 0.5f));
        positionIndex++;

        for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
        {
          float x = unitCircleVertices[j];
          float y = unitCircleVertices[j+1];

          batch.addVertex(matrix.transformPoint(x * baseRadius, y * baseRadius, z), normalMatrix * glm::vec3(0, 0, -1), glm::vec2(-x * 0.5f + 0.5f, -y * 0.5f + 0.5f));
          positionIndex++;
        }

        //

        int topPositionIndex = positionIndex;

        z = height * 0.5f;
        batch.addVertex(matrix.transformPoint(0, 0, z), normalMatrix * glm::vec3(0, 0, 1), glm::vec2(0.5f, 0.5f));
        positionIndex++;

        for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
        {
          float x = unitCircleVertices[j];
          float y = unitCircleVertices[j+1];

          batch.addVertex(matrix.transformPoint(x * topRadius, y * topRadius, z), normalMatrix * glm::vec3(0, 0, 1), glm::vec2(x * 0.5f + 0.5f, -y * 0.5f + 0.5f));
          positionIndex++;
        }

        //

        for (int i = 0; i < stackCount; ++i)
        {
          int k1 = i * (sectorCount + 1);
          int k2 = k1 + sectorCount + 1;

          for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
          {
            if (frontFace == CCW)
            {
              batch.addIndices(k1, k2, k1 + 1, k2, k2 + 1, k1 + 1);
            }
            else
            {
              batch.addIndices(k1, k1 + 1, k2, k2, k1 + 1, k2 + 1);
            }
          }
        }

        for (int i = 0, k = basePositionIndex + 1; i < sectorCount; ++i, ++k)
        {
          if (i < (sectorCount - 1))
          {
            if (frontFace == CCW)
            {
              batch.addIndices(basePositionIndex, k, k + 1);
            }
            else
            {
              batch.addIndices(basePositionIndex, k + 1, k);
            }
          }
          else
          {
            if (frontFace == CCW)
            {
              batch.addIndices(basePositionIndex, k, basePositionIndex + 1);
            }
            else
            {
              batch.addIndices(basePositionIndex, basePositionIndex + 1, k);
            }
          }
        }

        for (int i = 0, k = topPositionIndex + 1; i < sectorCount; ++i, ++k)
        {
          if (i < (sectorCount - 1))
          {
            if (frontFace == CCW)
            {
              batch.addIndices(topPositionIndex, k + 1, k);
            }
            else
            {
              batch.addIndices(topPositionIndex, k, k + 1);
            }
          }
          else
          {
            if (frontFace == CCW)
            {
              batch.addIndices(topPositionIndex, topPositionIndex + 1, k);
            }
            else
            {
              batch.addIndices(topPositionIndex, k, topPositionIndex + 1);
            }
          }
        }

        batch.incrementIndices(batch.vertexCount() - initialVertexCount);
      }

      template <>
      void Cylinder::append(IndexedVertexBatch<XYZ.N.RGBA, GLuint> &batch, const Matrix &matrix) const
      {
        auto normalMatrix = matrix.getNormalMatrix();
        auto initialVertexCount = batch.vertexCount();
        auto unitCircleVertices = getUnitCircleVertices();
        auto sideNormals = getSideNormals();
        int positionIndex = 0;

        for (int i = 0; i <= stackCount; ++i)
        {
          float z = -(height * 0.5f) + (float)i / stackCount * height;
          float radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);

          for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
          {
            float x = unitCircleVertices[k];
            float y = unitCircleVertices[k+1];

            batch.addVertex(matrix.transformPoint(x * radius, y * radius, z), normalMatrix * glm::vec3(sideNormals[k], sideNormals[k+1], sideNormals[k+2]), color);
            positionIndex++;
          }
        }

        //

        int basePositionIndex = positionIndex;

        float z = -height * 0.5f;
        batch.addVertex(matrix.transformPoint(0, 0, z), normalMatrix * glm::vec3(0, 0, -1), color);
        positionIndex++;

        for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
        {
          float x = unitCircleVertices[j];
          float y = unitCircleVertices[j+1];

          batch.addVertex(matrix.transformPoint(x * baseRadius, y * baseRadius, z), normalMatrix * glm::vec3(0, 0, -1), color);
          positionIndex++;
        }

        //

        int topPositionIndex = positionIndex;

        z = height * 0.5f;
        batch.addVertex(matrix.transformPoint(0, 0, z), normalMatrix * glm::vec3(0, 0, 1), color);
        positionIndex++;

        for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
        {
          float x = unitCircleVertices[j];
          float y = unitCircleVertices[j+1];

          batch.addVertex(matrix.transformPoint(x * topRadius, y * topRadius, z), normalMatrix * glm::vec3(0, 0, 1), color);
          positionIndex++;
        }

        //

        for (int i = 0; i < stackCount; ++i)
        {
          int k1 = i * (sectorCount + 1);
          int k2 = k1 + sectorCount + 1;

          for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
          {
            if (frontFace == CCW)
            {
              batch.addIndices(k1, k2, k1 + 1, k2, k2 + 1, k1 + 1);
            }
            else
            {
              batch.addIndices(k1, k1 + 1, k2, k2, k1 + 1, k2 + 1);
            }
          }
        }

        for (int i = 0, k = basePositionIndex + 1; i < sectorCount; ++i, ++k)
        {
          if (i < (sectorCount - 1))
          {
            if (frontFace == CCW)
            {
              batch.addIndices(basePositionIndex, k, k + 1);
            }
            else
            {
              batch.addIndices(basePositionIndex, k + 1, k);
            }
          }
          else
          {
            if (frontFace == CCW)
            {
              batch.addIndices(basePositionIndex, k, basePositionIndex + 1);
            }
            else
            {
              batch.addIndices(basePositionIndex, basePositionIndex + 1, k);
            }
          }
        }

        for (int i = 0, k = topPositionIndex + 1; i < sectorCount; ++i, ++k)
        {
          if (i < (sectorCount - 1))
          {
            if (frontFace == CCW)
            {
              batch.addIndices(topPositionIndex, k + 1, k);
            }
            else
            {
              batch.addIndices(topPositionIndex, k, k + 1);
            }
          }
          else
          {
            if (frontFace == CCW)
            {
              batch.addIndices(topPositionIndex, topPositionIndex + 1, k);
            }
            else
            {
              batch.addIndices(topPositionIndex, k, topPositionIndex + 1);
            }
          }
        }

        batch.incrementIndices(batch.vertexCount() - initialVertexCount);
      }

      template <>
      void Cylinder::append(IndexedVertexBatch<XYZ.N.UV.RGBA, GLuint> &batch, const Matrix &matrix) const
      {
        auto normalMatrix = matrix.getNormalMatrix();
        auto initialVertexCount = batch.vertexCount();
        auto unitCircleVertices = getUnitCircleVertices();
        auto sideNormals = getSideNormals();
        int positionIndex = 0;

        for (int i = 0; i <= stackCount; ++i)
        {
          float z = -(height * 0.5f) + (float)i / stackCount * height;
          float radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);
          float t = 1.0f - (float)i / stackCount;

          for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
          {
            float x = unitCircleVertices[k];
            float y = unitCircleVertices[k+1];

            batch.addVertex(matrix.transformPoint(x * radius, y * radius, z), normalMatrix * glm::vec3(sideNormals[k], sideNormals[k+1], sideNormals[k+2]), glm::vec2((float)j / sectorCount, t), color);
            positionIndex++;
          }
        }

        //

        int basePositionIndex = positionIndex;

        float z = -height * 0.5f;
        batch.addVertex(matrix.transformPoint(0, 0, z), normalMatrix * glm::vec3(0, 0, -1), glm::vec2(0.5f, 0.5f), color);
        positionIndex++;

        for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
        {
          float x = unitCircleVertices[j];
          float y = unitCircleVertices[j+1];

          batch.addVertex(matrix.transformPoint(x * baseRadius, y * baseRadius, z), normalMatrix * glm::vec3(0, 0, -1), glm::vec2(-x * 0.5f + 0.5f, -y * 0.5f + 0.5f), color);
          positionIndex++;
        }

        //

        int topPositionIndex = positionIndex;

        z = height * 0.5f;
        batch.addVertex(matrix.transformPoint(0, 0, z), normalMatrix * glm::vec3(0, 0, 1), glm::vec2(0.5f, 0.5f), color);
        positionIndex++;

        for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
        {
          float x = unitCircleVertices[j];
          float y = unitCircleVertices[j+1];

          batch.addVertex(matrix.transformPoint(x * topRadius, y * topRadius, z), normalMatrix * glm::vec3(0, 0, 1), glm::vec2(x * 0.5f + 0.5f, -y * 0.5f + 0.5f), color);
          positionIndex++;
        }

        //

        for (int i = 0; i < stackCount; ++i)
        {
          int k1 = i * (sectorCount + 1);
          int k2 = k1 + sectorCount + 1;

          for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
          {
            if (frontFace == CCW)
            {
              batch.addIndices(k1, k2, k1 + 1, k2, k2 + 1, k1 + 1);
            }
            else
            {
              batch.addIndices(k1, k1 + 1, k2, k2, k1 + 1, k2 + 1);
            }
          }
        }

        for (int i = 0, k = basePositionIndex + 1; i < sectorCount; ++i, ++k)
        {
          if (i < (sectorCount - 1))
          {
            if (frontFace == CCW)
            {
              batch.addIndices(basePositionIndex, k, k + 1);
            }
            else
            {
              batch.addIndices(basePositionIndex, k + 1, k);
            }
          }
          else
          {
            if (frontFace == CCW)
            {
              batch.addIndices(basePositionIndex, k, basePositionIndex + 1);
            }
            else
            {
              batch.addIndices(basePositionIndex, basePositionIndex + 1, k);
            }
          }
        }

        for (int i = 0, k = topPositionIndex + 1; i < sectorCount; ++i, ++k)
        {
          if (i < (sectorCount - 1))
          {
            if (frontFace == CCW)
            {
              batch.addIndices(topPositionIndex, k + 1, k);
            }
            else
            {
              batch.addIndices(topPositionIndex, k, k + 1);
            }
          }
          else
          {
            if (frontFace == CCW)
            {
              batch.addIndices(topPositionIndex, topPositionIndex + 1, k);
            }
            else
            {
              batch.addIndices(topPositionIndex, k, topPositionIndex + 1);
            }
          }
        }

        batch.incrementIndices(batch.vertexCount() - initialVertexCount);
      }

      vector<float> Cylinder::getUnitCircleVertices() const
      {
        float sectorStep = TWO_PI / sectorCount;
        vector<float> unitCircleVertices;

        for (int i = 0; i <= sectorCount; ++i)
        {
          float sectorAngle = i * sectorStep;
          unitCircleVertices.push_back(cosf(sectorAngle));
          unitCircleVertices.push_back(sinf(sectorAngle));
          unitCircleVertices.push_back(0);
        }

        return unitCircleVertices;
      }

      vector<float> Cylinder::getSideNormals() const
      {
        float zAngle = atan2f(baseRadius - topRadius, height);
        float x0 = cosf(zAngle);
        float y0 = 0;
        float z0 = sinf(zAngle);

        float sectorStep = TWO_PI / sectorCount;
        vector<float> normals;

        for (int i = 0; i <= sectorCount; ++i)
        {
          float sectorAngle = i * sectorStep;
          normals.push_back(cosf(sectorAngle) * x0 - sinf(sectorAngle) * y0);
          normals.push_back(sinf(sectorAngle) * x0 + cosf(sectorAngle) * y0);
          normals.push_back(z0);
        }

        return normals;
      }
    }
  }
}
