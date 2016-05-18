#pragma once

#include "math/Rect.h"
#include "gl/Batch.h"
#include "gl/Matrix.h"

#include "tesselator.h"

namespace chr
{
  namespace gl
  {
    class Triangulator
    {
    public:
      enum
      {
        CAPTURE_NONE = 0,
        CAPTURE_FRONT = 1,
        CAPTURE_BACK = 2,
        CAPTURE_HEIGHT = 4,
        CAPTURE_ALL = 7
      };

      struct Segment
      {
        glm::vec2 p1;
        glm::vec2 p2;
        glm::vec2 tangeant;

        Segment(const glm::vec2 &p1, const glm::vec2 &p2, glm::vec2 &tangeant)
        :
        p1(p1),
        p2(p2),
        tangeant(tangeant)
        {}
      };

      Triangulator();
      ~Triangulator();

      Triangulator& setWindingRule(int windingRule);
      Triangulator& setFrontFace(GLenum mode);

      Triangulator& setTextureOffset(const glm::vec2 &offset);
      Triangulator& setTextureOffset(float x, float y);
      Triangulator& setTextureScale(float scale);

      Triangulator& setColor(const glm::vec4 &color);
      Triangulator& setColor(float r, float g, float b, float a);

      Triangulator& setContourCapture(int contourCapture);
      void exportContours(IndexedVertexBatch<XYZ> &batch, Matrix &matrix) const;

      template<int Orientation = GL_CCW>
      Triangulator& add(const math::Rectf &rect);

      Triangulator& add(const std::vector<std::vector<glm::vec2>> &polygons);
      Triangulator& add(const std::vector<glm::vec2> &polygon);

      template<int V = XYZ>
      void stamp(IndexedVertexBatch<V> &batch, Matrix &matrix);

      template<int V = XYZ>
      void extrude(IndexedVertexBatch<V> &batch, Matrix &matrix, float distance);

    protected:
      TESStesselator *tess;
      int allocated = 0;

      int windingRule = TESS_WINDING_ODD;
      GLenum frontFace = GL_CCW;
      glm::vec2 textureOffset;
      float textureScale = 1;
      glm::vec4 color = { 1, 1, 1, 1 };

      int contourCapture = CAPTURE_NONE;
      std::vector<std::vector<glm::vec2>> contours;
      float extrudedDistance = 0;

      void captureContours();

      glm::vec2 getTextureCoords(const gl::Texture &texture, const glm::vec2 &xy) const;

      template<int V = XYZ, typename... Args>
      void performStamp(IndexedVertexBatch<V> &batch, Matrix &matrix, Args&&... args)
      {
        extrudedDistance = 0;

        if (contourCapture)
        {
          captureContours();
        }

        // ---

        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

        // ---

        auto vertices = (glm::vec2*)tessGetVertices(tess);
        auto vertexCount = tessGetVertexCount(tess);

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(matrix.transformPoint(vertices[i]), std::forward<Args>(args)...);
        }

        // ---

        auto indices = (int*)tessGetElements(tess);
        auto indexCount =  tessGetElementCount(tess) * 3;

        bool CW = (frontFace == GL_CW);

        for (int i = 0; i < indexCount; i += 3)
        {
          batch.addIndices(
            indices[i + (CW ? 2 : 0)],
            indices[i + 1],
            indices[i + (CW ? 0 : 2)]);
        }

        batch.incrementIndices(vertexCount);
      }

      template<int V = XYZ, typename... Args>
      void performStampWithTexture(IndexedVertexBatch<V> &batch, Matrix &matrix, Args&&... args)
      {
        extrudedDistance = 0;

        if (contourCapture)
        {
          captureContours();
        }

        // ---

        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

        // ---

        auto vertices = (glm::vec2*)tessGetVertices(tess);
        auto vertexCount = tessGetVertexCount(tess);

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(matrix.transformPoint(vertices[i]), getTextureCoords(batch.texture, vertices[i]), std::forward<Args>(args)...);
        }

        // ---

        auto indices = (int*)tessGetElements(tess);
        auto indexCount =  tessGetElementCount(tess) * 3;

        bool CW = (frontFace == GL_CW);

        for (int i = 0; i < indexCount; i += 3)
        {
          batch.addIndices(
            indices[i + (CW ? 2 : 0)],
            indices[i + 1],
            indices[i + (CW ? 0 : 2)]);
        }

        batch.incrementIndices(vertexCount);
      }

      template<int V = XYZ, typename... Args>
      void performExtrude(IndexedVertexBatch<V> &batch, Matrix &matrix, float distance, Args&&... args)
      {
        extrudedDistance = distance;
        bool sign = (distance > 0);

        tessTesselate(tess, windingRule, TESS_BOUNDARY_CONTOURS, 0, 0, 0);

        auto vertices = (glm::vec2*)tessGetVertices(tess);
        auto elements = tessGetElements(tess);
        auto elementCount = tessGetElementCount(tess);

        if (contourCapture)
        {
          contours.clear();
          contours.reserve(elementCount);
        }

        for (auto i = 0; i < elementCount; i++)
        {
          const auto base = elements[i << 1];
          const auto count = elements[(i << 1) + 1];

          if (contourCapture)
          {
            contours.emplace_back();
            contours.back().reserve(count);
          }

          for (int j = 0; j < count; j++)
          {
            auto &p0 = vertices[base + j];
            auto &p1 = vertices[base + (j + 1) % count];

            if (contourCapture)
            {
              contours.back().emplace_back(vertices[base + j]);
            }

            batch
              .addVertex(matrix.transformPoint(p0), std::forward<Args>(args)...)
              .addVertex(matrix.transformPoint(p1), std::forward<Args>(args)...)
              .addVertex(matrix.transformPoint(glm::vec3(p1, distance)), std::forward<Args>(args)...)
              .addVertex(matrix.transformPoint(glm::vec3(p0, distance)), std::forward<Args>(args)...);

            if (sign)
            {
              batch.addIndices(0, 1, 2, 2, 3, 0);
            }
            else
            {
              batch.addIndices(0, 3, 2, 2, 1, 0);
            }

            batch.incrementIndices(4);
          }

          /*
           * NECESSARY TO ADD THE CONTOURS BACK FOR FURTHER tessTesselate OPERATIONS
           */
          tessAddContour(tess, 2, &vertices[base], sizeof(glm::vec2), count);
        }

        // ---

        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

        vertices = (glm::vec2*)tessGetVertices(tess);
        auto vertexCount = tessGetVertexCount(tess);

        auto indices = (int *) tessGetElements(tess);
        auto indexCount = tessGetElementCount(tess) * 3;

        // ---

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(matrix.transformPoint(vertices[i]), std::forward<Args>(args)...);
        }

        for (int i = 0; i < indexCount; i += 3)
        {
          batch.addIndices(
            indices[i + (sign ? 2 : 0)],
            indices[i + 1],
            indices[i + (sign ? 0 : 2)]);
        }

        batch.incrementIndices(vertexCount);

        // ---

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(matrix.transformPoint(glm::vec3(vertices[i], distance)), std::forward<Args>(args)...);
        }

        for (int i = 0; i < indexCount; i += 3)
        {
          batch.addIndices(
            indices[i + (sign ? 0 : 2)],
            indices[i + 1],
            indices[i + (sign ? 2 : 0)]);
        }

        batch.incrementIndices(vertexCount);
      }

      template<int V = XYZ, typename... Args>
      void performExtrudeWithNormals(IndexedVertexBatch<V> &batch, Matrix &matrix, float distance, Args&&... args)
      {
        extrudedDistance = distance;
        bool sign = (distance > 0);

        tessTesselate(tess, windingRule, TESS_BOUNDARY_CONTOURS, 0, 0, 0);

        auto vertices = (glm::vec2*)tessGetVertices(tess);
        auto elements = tessGetElements(tess);
        auto elementCount = tessGetElementCount(tess);

        if (contourCapture)
        {
          contours.clear();
          contours.reserve(elementCount);
        }

        for (auto i = 0; i < elementCount; i++)
        {
          const auto base = elements[i << 1];
          const auto count = elements[(i << 1) + 1];

          if (contourCapture)
          {
            contours.emplace_back();
            contours.back().reserve(count);
          }

          for (int j = 0; j < count; j++)
          {
            auto &p0 = vertices[base + j];
            auto &p1 = vertices[base + (j + 1) % count];

            auto tangeant = glm::normalize(p1 - p0).yx() * glm::vec2(-1, +1);
            auto normal = matrix.transformNormal(glm::vec3(tangeant, 0));

            if (contourCapture)
            {
              contours.back().emplace_back(vertices[base + j]);
            }

            batch
              .addVertex(matrix.transformPoint(p0), normal, std::forward<Args>(args)...)
              .addVertex(matrix.transformPoint(p1), normal, std::forward<Args>(args)...)
              .addVertex(matrix.transformPoint(glm::vec3(p1, distance)), normal, std::forward<Args>(args)...)
              .addVertex(matrix.transformPoint(glm::vec3(p0, distance)), normal, std::forward<Args>(args)...);

            if (sign)
            {
              batch.addIndices(0, 1, 2, 2, 3, 0);
            }
            else
            {
              batch.addIndices(0, 3, 2, 2, 1, 0);
            }

            batch.incrementIndices(4);
          }

          /*
           * NECESSARY TO ADD THE CONTOURS BACK FOR FURTHER tessTesselate OPERATIONS
           */
          tessAddContour(tess, 2, &vertices[base], sizeof(glm::vec2), count);
        }

        // ---

        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

        vertices = (glm::vec2*)tessGetVertices(tess);
        auto vertexCount = tessGetVertexCount(tess);

        auto indices = (int *) tessGetElements(tess);
        auto indexCount = tessGetElementCount(tess) * 3;

        // ---

        auto normal1 = matrix.transformNormal(0, 0, sign ? -1 : +1);

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(matrix.transformPoint(vertices[i]), normal1, std::forward<Args>(args)...);
        }

        for (int i = 0; i < indexCount; i += 3)
        {
          batch.addIndices(
            indices[i + (sign ? 2 : 0)],
            indices[i + 1],
            indices[i + (sign ? 0 : 2)]);
        }

        batch.incrementIndices(vertexCount);

        // ---

        auto normal2 = matrix.transformNormal(0, 0, sign ? +1 : -1);

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(matrix.transformPoint(glm::vec3(vertices[i], distance)), normal2, std::forward<Args>(args)...);
        }

        for (int i = 0; i < indexCount; i += 3)
        {
          batch.addIndices(
            indices[i + (sign ? 0 : 2)],
            indices[i + 1],
            indices[i + (sign ? 2 : 0)]);
        }

        batch.incrementIndices(vertexCount);
      }
    };
  }
}
