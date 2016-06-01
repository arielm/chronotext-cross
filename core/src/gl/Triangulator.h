#pragma once

#include "path/Shape.h"
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
      enum CaptureType
      {
        CAPTURE_NONE = 0,
        CAPTURE_FRONT = 1,
        CAPTURE_BACK = 2,
        CAPTURE_HEIGHT = 4,
        CAPTURE_ALL = 7
      };

      Triangulator();
      ~Triangulator();

      Triangulator& setWindingRule(TessWindingRule windingRule);
      Triangulator& setFrontFace(GLenum mode);

      Triangulator& setTextureOffset(const glm::vec2 &offset);
      Triangulator& setTextureOffset(float x, float y);
      Triangulator& setTextureScale(float scale);

      Triangulator& setColor(const glm::vec4 &color);
      Triangulator& setColor(float r, float g, float b, float a);

      Triangulator& setContourCapture(CaptureType capture);
      void exportContours(IndexedVertexBatch<XYZ> &batch, Matrix &matrix) const;

      Triangulator& add(const path::Shape &shape);
      Triangulator& add(const std::vector<glm::vec2> &polyline);

      template<int V = XYZ>
      void fill(IndexedVertexBatch<V> &batch);

      template<int V = XYZ>
      void fill(IndexedVertexBatch<V> &batch, Matrix &matrix);

      template<int V = XYZ>
      void extrude(IndexedVertexBatch<V> &batch, Matrix &matrix, float distance);

    protected:
      TESStesselator *tess;
      int allocated = 0;

      TessWindingRule windingRule = TESS_WINDING_ODD;
      GLenum frontFace = GL_CCW;
      glm::vec2 textureOffset;
      float textureScale = 1;
      glm::vec4 color = { 1, 1, 1, 1 };

      int contourCapture = CAPTURE_NONE;
      std::vector<std::vector<glm::vec2>> contours;
      float extrudedDistance = 0;

      void captureContours();

      glm::vec2 getTextureCoords(const gl::Texture &texture, const glm::vec2 &xy) const;
      static TessWindingRule convert(path::Shape::FillRule fillRule);

      // ---

      template<int V = XYZ, typename... Args>
      void performFill(IndexedVertexBatch<V> &batch, Args&&... args)
      {
        extrudedDistance = 0;

        if (contourCapture)
        {
          captureContours();
        }

        // ---

        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

        auto vertices = (glm::vec2*)tessGetVertices(tess);
        auto vertexCount = tessGetVertexCount(tess);

        auto elements = tessGetElements(tess);
        auto elementCount = tessGetElementCount(tess) * 3;

        // ---

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(glm::vec3(vertices[i], 0), std::forward<Args>(args)...);
        }

        // ---

        bool CW = (frontFace == GL_CW);

        for (int i = 0; i < elementCount; i += 3)
        {
          batch.addIndices(
            elements[i + (CW ? 2 : 0)],
            elements[i + 1],
            elements[i + (CW ? 0 : 2)]);
        }

        batch.incrementIndices(vertexCount);
      }

      template<int V = XYZ, typename... Args>
      void performFillWithTexture(IndexedVertexBatch<V> &batch, Args&&... args)
      {
        extrudedDistance = 0;

        if (contourCapture)
        {
          captureContours();
        }

        // ---

        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

        auto vertices = (glm::vec2*)tessGetVertices(tess);
        auto vertexCount = tessGetVertexCount(tess);

        auto elements = tessGetElements(tess);
        auto elementCount = tessGetElementCount(tess) * 3;

        // ---

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(
            glm::vec3(vertices[i], 0),
            getTextureCoords(batch.texture, vertices[i]),
            std::forward<Args>(args)...);
        }

        // ---

        bool CW = (frontFace == GL_CW);

        for (int i = 0; i < elementCount; i += 3)
        {
          batch.addIndices(
            elements[i + (CW ? 2 : 0)],
            elements[i + 1],
            elements[i + (CW ? 0 : 2)]);
        }

        batch.incrementIndices(vertexCount);
      }

      // ---

      template<int V = XYZ, typename... Args>
      void performFill(IndexedVertexBatch<V> &batch, Matrix &matrix, Args&&... args)
      {
        extrudedDistance = 0;

        if (contourCapture)
        {
          captureContours();
        }

        // ---

        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

        auto vertices = (glm::vec2*)tessGetVertices(tess);
        auto vertexCount = tessGetVertexCount(tess);

        auto elements = tessGetElements(tess);
        auto elementCount = tessGetElementCount(tess) * 3;

        // ---

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(matrix.transformPoint(vertices[i]), std::forward<Args>(args)...);
        }

        // ---

        bool CW = (frontFace == GL_CW);

        for (int i = 0; i < elementCount; i += 3)
        {
          batch.addIndices(
            elements[i + (CW ? 2 : 0)],
            elements[i + 1],
            elements[i + (CW ? 0 : 2)]);
        }

        batch.incrementIndices(vertexCount);
      }

      template<int V = XYZ, typename... Args>
      void performFillWithTexture(IndexedVertexBatch<V> &batch, Matrix &matrix, Args&&... args)
      {
        extrudedDistance = 0;

        if (contourCapture)
        {
          captureContours();
        }

        // ---

        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

        auto vertices = (glm::vec2*)tessGetVertices(tess);
        auto vertexCount = tessGetVertexCount(tess);

        auto elements = tessGetElements(tess);
        auto elementCount = tessGetElementCount(tess) * 3;

        // ---

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(
            matrix.transformPoint(vertices[i]),
            getTextureCoords(batch.texture, vertices[i]),
            std::forward<Args>(args)...);
        }

        // ---

        bool CW = (frontFace == GL_CW);

        for (int i = 0; i < elementCount; i += 3)
        {
          batch.addIndices(
            elements[i + (CW ? 2 : 0)],
            elements[i + 1],
            elements[i + (CW ? 0 : 2)]);
        }

        batch.incrementIndices(vertexCount);
      }

      template<int V = XYZ, typename... Args>
      void performFillWithNormalsAndTexture(IndexedVertexBatch<V> &batch, Matrix &matrix, const glm::vec3 &normal, Args&&... args)
      {
        extrudedDistance = 0;

        if (contourCapture)
        {
          captureContours();
        }

        // ---

        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

        auto vertices = (glm::vec2*)tessGetVertices(tess);
        auto vertexCount = tessGetVertexCount(tess);

        auto elements = tessGetElements(tess);
        auto elementCount = tessGetElementCount(tess) * 3;

        // ---

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(
            matrix.transformPoint(vertices[i]),
            normal,
            getTextureCoords(batch.texture, vertices[i]),
            std::forward<Args>(args)...);
        }

        // ---

        bool CW = (frontFace == GL_CW);

        for (int i = 0; i < elementCount; i += 3)
        {
          batch.addIndices(
            elements[i + (CW ? 2 : 0)],
            elements[i + 1],
            elements[i + (CW ? 0 : 2)]);
        }

        batch.incrementIndices(vertexCount);
      }

      // ---

      template<int V = XYZ, typename... Args>
      void performExtrude(IndexedVertexBatch<V> &batch, Matrix &matrix, float distance, Args&&... args)
      {
        extrudedDistance = distance;
        bool CW = ((frontFace == GL_CW) && (distance > 0)) || ((frontFace == GL_CCW) && (distance < 0));

        tessTesselate(tess, windingRule, TESS_BOUNDARY_CONTOURS, 0, 0, 0);

        auto contourVertices = (glm::vec2*)tessGetVertices(tess);
        auto contourVertexCount = tessGetVertexCount(tess);
        auto contourVertexIndices = tessGetVertexIndices(tess);

        auto contourElements = tessGetElements(tess);
        auto contourElementCount = tessGetElementCount(tess);

        if (contourCapture)
        {
          contours.clear();
          contours.reserve(contourElementCount);
        }

        // ---

        /*
         * HANDLING DUPLICATE VERTICES
         * (AS A CONSEQUENCE OF INTERSECTING CONTOURS)
         */

        std::map<glm::vec2, int> uniqueIndices;
        int map1[contourVertexCount];

        for (auto i = 0; i < contourVertexCount; i++)
        {
          map1[i] = i;
        }

        for (int i = 0; i < contourVertexCount; i++)
        {
          /*
           * INDICES MARKED AS "TESS_UNDEF" CORRESPOND TO INTERSECTIONS
           */
          if (contourVertexIndices[i] == TESS_UNDEF)
          {
            auto found = uniqueIndices.find(contourVertices[i]);

            if (found != uniqueIndices.end())
            {
              map1[i] = found->second;
            }
            else
            {
              map1[i] = i;
              uniqueIndices.emplace(contourVertices[i], i);
            }
          }
        }

        // ---

        std::vector<std::vector<int>> contourIndices;
        contourIndices.reserve(contourElementCount);

        for (auto i = 0; i < contourElementCount; i++)
        {
          const auto base = contourElements[i * 2];
          const auto count = contourElements[(i * 2) + 1];

          if (contourCapture)
          {
            contours.emplace_back();
            contours.back().reserve(count);

            for (int j = 0; j < count; j++)
            {
              contours.back().emplace_back(contourVertices[base + j]);
            }
          }

          /*
           * EACH OF THE LISTS OF CONTOUR INDICES GENERATED
           * WILL REFER TO UNIQUE VERTICES
           */

          contourIndices.emplace_back();
          contourIndices.back().reserve(count);

          for (int j = 0; j < count; j++)
          {
            contourIndices.back().emplace_back(map1[base + j]);
          }

          /*
           * NECESSARY TO ADD THE CONTOURS BACK FOR FURTHER tessTesselate OPERATIONS
           */
          tessAddContour(tess, 2, &contourVertices[base], sizeof(glm::vec2), count);
        }

        // ---

        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

        auto vertices = (glm::vec2*)tessGetVertices(tess);
        auto vertexCount = tessGetVertexCount(tess);
        auto vertexIndices = tessGetVertexIndices(tess);

        auto elements = tessGetElements(tess);
        auto elementCount = tessGetElementCount(tess) * 3;

        // ---

        /*
         * PROPERLY MAPPING BETWEEN THE PREVIOUS CONTOUR
         * VERTICES AND THE CURRENT TRIANGULATION VERTICES
         */

        int map2[contourVertexCount]; // NO NEED TO PRE-FILL

        for (auto i = 0; i < vertexCount; i++)
        {
          map2[map1[vertexIndices[i]]] = i;
        }

        for (const auto &contour : contourIndices)
        {
          auto size = contour.size();

          for (auto i = 0; i < size; i++)
          {
            auto i0 = map2[contour[i]];
            auto i1 = map2[contour[(i + 1) % size]];

            if (CW)
            {
              batch
                .addIndices(i0, i0 + vertexCount, i1 + vertexCount)
                .addIndices(i1 + vertexCount, i1, i0);
            }
            else
            {
              batch
                .addIndices(i0, i1, i1 + vertexCount)
                .addIndices(i1 + vertexCount, i0 + vertexCount, i0);
            }
          }
        }

        // ---

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(matrix.transformPoint(vertices[i]), std::forward<Args>(args)...);
        }

        for (int i = 0; i < elementCount; i += 3)
        {
          batch.addIndices(
            elements[i + (CW ? 0 : 2)],
            elements[i + 1],
            elements[i + (CW ? 2 : 0)]);
        }

        batch.incrementIndices(vertexCount);

        // ---

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(matrix.transformPoint(glm::vec3(vertices[i], distance)), std::forward<Args>(args)...);
        }

        for (int i = 0; i < elementCount; i += 3)
        {
          batch.addIndices(
            elements[i + (CW ? 2 : 0)],
            elements[i + 1],
            elements[i + (CW ? 0 : 2)]);
        }

        batch.incrementIndices(vertexCount);
      }

      template<int V = XYZ, typename... Args>
      void performExtrudeWithTexture(IndexedVertexBatch<V> &batch, Matrix &matrix, float distance, Args&&... args)
      {
        extrudedDistance = distance;
        bool CW = ((frontFace == GL_CW) && (distance > 0)) || ((frontFace == GL_CCW) && (distance < 0));

        tessTesselate(tess, windingRule, TESS_BOUNDARY_CONTOURS, 0, 0, 0);

        auto contourVertices = (glm::vec2*)tessGetVertices(tess);
        auto contourVertexCount = tessGetVertexCount(tess);

        auto contourElements = tessGetElements(tess);
        auto contourElementCount = tessGetElementCount(tess);

        if (contourCapture)
        {
          contours.clear();
          contours.reserve(contourElementCount);
        }

        for (auto i = 0; i < contourElementCount; i++)
        {
          const auto base = contourElements[i << 1];
          const auto count = contourElements[(i << 1) + 1];

          float length = 0;

          if (contourCapture)
          {
            contours.emplace_back();
            contours.back().reserve(count);
          }

          for (int j = 0; j < count; j++)
          {
            auto &p0 = contourVertices[base + j];
            auto &p1 = contourVertices[base + (j + 1) % count];

            float length0 = length;
            length += glm::length(p1 - p0);

            if (contourCapture)
            {
              contours.back().emplace_back(p0);
            }

            batch
              .addVertex(
                matrix.transformPoint(p0),
                getTextureCoords(batch.texture, glm::vec2(length0, 0)),
                std::forward<Args>(args)...)
              .addVertex(
                matrix.transformPoint(p1),
                getTextureCoords(batch.texture, glm::vec2(length, 0)),
                std::forward<Args>(args)...)
              .addVertex(
                matrix.transformPoint(glm::vec3(p1, distance)),
                getTextureCoords(batch.texture, glm::vec2(length, distance)),
                std::forward<Args>(args)...)
              .addVertex(
                matrix.transformPoint(glm::vec3(p0, distance)),
                getTextureCoords(batch.texture, glm::vec2(length0, distance)),
                std::forward<Args>(args)...);

            if (CW)
            {
              batch.addIndices(0, 3, 2, 2, 1, 0);
            }
            else
            {
              batch.addIndices(0, 1, 2, 2, 3, 0);
            }

            batch.incrementIndices(4);
          }

          /*
           * NECESSARY TO ADD THE CONTOURS BACK FOR FURTHER tessTesselate OPERATIONS
           */
          tessAddContour(tess, 2, &contourVertices[base], sizeof(glm::vec2), count);
        }

        // ---

        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

        auto vertices = (glm::vec2*)tessGetVertices(tess);
        auto vertexCount = tessGetVertexCount(tess);

        auto elements = tessGetElements(tess);
        auto elementCount = tessGetElementCount(tess) * 3;

        // ---

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(
            matrix.transformPoint(vertices[i]),
            getTextureCoords(batch.texture, vertices[i]),
            std::forward<Args>(args)...);
        }

        for (int i = 0; i < elementCount; i += 3)
        {
          batch.addIndices(
            elements[i + (CW ? 0 : 2)],
            elements[i + 1],
            elements[i + (CW ? 2 : 0)]);
        }

        batch.incrementIndices(vertexCount);

        // ---

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(
            matrix.transformPoint(glm::vec3(vertices[i], distance)),
            getTextureCoords(batch.texture, vertices[i]),
            std::forward<Args>(args)...);
        }

        for (int i = 0; i < elementCount; i += 3)
        {
          batch.addIndices(
            elements[i + (CW ? 2 : 0)],
            elements[i + 1],
            elements[i + (CW ? 0 : 2)]);
        }

        batch.incrementIndices(vertexCount);
      }

      template<int V = XYZ, typename... Args>
      void performExtrudeWithNormals(IndexedVertexBatch<V> &batch, Matrix &matrix, float distance, Args&&... args)
      {
        extrudedDistance = distance;

        bool CW = ((frontFace == GL_CW) && (distance > 0)) || ((frontFace == GL_CCW) && (distance < 0));
        int sign = (frontFace == GL_CW) ? + 1 : -1;

        tessTesselate(tess, windingRule, TESS_BOUNDARY_CONTOURS, 0, 0, 0);

        auto contourVertices = (glm::vec2*)tessGetVertices(tess);
        auto contourElements = tessGetElements(tess);
        auto contourElementCount = tessGetElementCount(tess);

        if (contourCapture)
        {
          contours.clear();
          contours.reserve(contourElementCount);
        }

        for (auto i = 0; i < contourElementCount; i++)
        {
          const auto base = contourElements[i << 1];
          const auto count = contourElements[(i << 1) + 1];

          if (contourCapture)
          {
            contours.emplace_back();
            contours.back().reserve(count);
          }

          for (int j = 0; j < count; j++)
          {
            auto &p0 = contourVertices[base + j];
            auto &p1 = contourVertices[base + (j + 1) % count];

            auto up = glm::normalize(p1 - p0).yx() * glm::vec2(-1, +1);
            auto normal = matrix.transformNormal(glm::vec3(up, 0));

            if (contourCapture)
            {
              contours.back().emplace_back(p0);
            }

            batch
              .addVertex(matrix.transformPoint(p0), normal, std::forward<Args>(args)...)
              .addVertex(matrix.transformPoint(p1), normal, std::forward<Args>(args)...)
              .addVertex(matrix.transformPoint(glm::vec3(p1, distance)), normal, std::forward<Args>(args)...)
              .addVertex(matrix.transformPoint(glm::vec3(p0, distance)), normal, std::forward<Args>(args)...);

            if (CW)
            {
              batch.addIndices(0, 3, 2, 2, 1, 0);
            }
            else
            {
              batch.addIndices(0, 1, 2, 2, 3, 0);
            }

            batch.incrementIndices(4);
          }

          /*
           * NECESSARY TO ADD THE CONTOURS BACK FOR FURTHER tessTesselate OPERATIONS
           */
          tessAddContour(tess, 2, &contourVertices[base], sizeof(glm::vec2), count);
        }

        // ---

        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

        auto vertices = (glm::vec2*)tessGetVertices(tess);
        auto vertexCount = tessGetVertexCount(tess);

        auto elements = tessGetElements(tess);
        auto elementCount = tessGetElementCount(tess) * 3;

        // ---

        auto normal1 = matrix.transformNormal(0, 0, sign * (CW ? -1 : +1));

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(matrix.transformPoint(vertices[i]), normal1, std::forward<Args>(args)...);
        }

        for (int i = 0; i < elementCount; i += 3)
        {
          batch.addIndices(
            elements[i + (CW ? 0 : 2)],
            elements[i + 1],
            elements[i + (CW ? 2 : 0)]);
        }

        batch.incrementIndices(vertexCount);

        // ---

        auto normal2 = matrix.transformNormal(0, 0, sign * (CW ? +1 : -1));

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(matrix.transformPoint(glm::vec3(vertices[i], distance)), normal2, std::forward<Args>(args)...);
        }

        for (int i = 0; i < elementCount; i += 3)
        {
          batch.addIndices(
            elements[i + (CW ? 2 : 0)],
            elements[i + 1],
            elements[i + (CW ? 0 : 2)]);
        }

        batch.incrementIndices(vertexCount);
      }

      template<int V = XYZ, typename... Args>
      void performExtrudeWithNormalsAndTexture(IndexedVertexBatch<V> &batch, Matrix &matrix, float distance, Args&&... args)
      {
        extrudedDistance = distance;

        bool CW = ((frontFace == GL_CW) && (distance > 0)) || ((frontFace == GL_CCW) && (distance < 0));
        int sign = (frontFace == GL_CW) ? + 1 : -1;

        tessTesselate(tess, windingRule, TESS_BOUNDARY_CONTOURS, 0, 0, 0);

        auto contourVertices = (glm::vec2*)tessGetVertices(tess);
        auto contourElements = tessGetElements(tess);
        auto contourElementCount = tessGetElementCount(tess);

        float length = 0;

        if (contourCapture)
        {
          contours.clear();
          contours.reserve(contourElementCount);
        }

        for (auto i = 0; i < contourElementCount; i++)
        {
          const auto base = contourElements[i << 1];
          const auto count = contourElements[(i << 1) + 1];

          if (contourCapture)
          {
            contours.emplace_back();
            contours.back().reserve(count);
          }

          for (int j = 0; j < count; j++)
          {
            auto &p0 = contourVertices[base + j];
            auto &p1 = contourVertices[base + (j + 1) % count];

            auto up = glm::normalize(p1 - p0).yx() * glm::vec2(-1, +1);
            auto normal = matrix.transformNormal(glm::vec3(up, 0));

            float length0 = length;
            length += glm::length(p1 - p0);

            if (contourCapture)
            {
              contours.back().emplace_back(p0);
            }

            batch
              .addVertex(
                matrix.transformPoint(p0),
                normal,
                getTextureCoords(batch.texture, glm::vec2(length0, 0)),
                std::forward<Args>(args)...)
              .addVertex(
                matrix.transformPoint(p1),
                normal,
                getTextureCoords(batch.texture, glm::vec2(length, 0)),
                std::forward<Args>(args)...)
              .addVertex(
                matrix.transformPoint(glm::vec3(p1, distance)),
                normal,
                getTextureCoords(batch.texture, glm::vec2(length, distance)),
                std::forward<Args>(args)...)
              .addVertex(
                matrix.transformPoint(glm::vec3(p0, distance)),
                normal,
                getTextureCoords(batch.texture, glm::vec2(length0, distance)),
                std::forward<Args>(args)...);

            if (CW)
            {
              batch.addIndices(0, 3, 2, 2, 1, 0);
            }
            else
            {
              batch.addIndices(0, 1, 2, 2, 3, 0);
            }

            batch.incrementIndices(4);
          }

          /*
           * NECESSARY TO ADD THE CONTOURS BACK FOR FURTHER tessTesselate OPERATIONS
           */
          tessAddContour(tess, 2, &contourVertices[base], sizeof(glm::vec2), count);
        }

        // ---

        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

        auto vertices = (glm::vec2*)tessGetVertices(tess);
        auto vertexCount = tessGetVertexCount(tess);

        auto elements = tessGetElements(tess);
        auto elementCount = tessGetElementCount(tess) * 3;

        // ---

        auto normal1 = matrix.transformNormal(0, 0, sign * (CW ? -1 : +1));

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(
            matrix.transformPoint(vertices[i]),
            normal1,
            getTextureCoords(batch.texture, vertices[i]),
            std::forward<Args>(args)...);
        }

        for (int i = 0; i < elementCount; i += 3)
        {
          batch.addIndices(
            elements[i + (CW ? 0 : 2)],
            elements[i + 1],
            elements[i + (CW ? 2 : 0)]);
        }

        batch.incrementIndices(vertexCount);

        // ---

        auto normal2 = matrix.transformNormal(0, 0, sign * (CW ? +1 : -1));

        for (int i = 0; i < vertexCount; i++)
        {
          batch.addVertex(
            matrix.transformPoint(glm::vec3(vertices[i], distance)),
            normal2,
            getTextureCoords(batch.texture, vertices[i]),
            std::forward<Args>(args)...);
        }

        for (int i = 0; i < elementCount; i += 3)
        {
          batch.addIndices(
            elements[i + (CW ? 2 : 0)],
            elements[i + 1],
            elements[i + (CW ? 0 : 2)]);
        }

        batch.incrementIndices(vertexCount);
      }
    };
  }
}
