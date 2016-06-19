#pragma once

#include "math/Utils.h"
#include "gl/Batch.h"

namespace chr
{
  namespace gl
  {
    class Revolver
    {
    public:
      enum
      {
        CAPTURE_NONE = 0,
        CAPTURE_ROWS = 1,
        CAPTURE_COLUMNS = 2,
        CAPTURE_CAP_FRONT = 4,
        CAPTURE_CAP_BACK = 8,
        CAPTURE_CAPS = 12,
        CAPTURE_ALL = 15
      };

      Revolver& setFrontFace(GLenum mode);

      Revolver& setTextureOffset(const glm::vec2 &offset);
      Revolver& setTextureOffset(float x, float y);
      Revolver& setTextureScale(float scale);

      Revolver& setColor(const glm::vec4 &color);
      Revolver& setColor(float r, float g, float b, float a);

      Revolver& setCapFront(bool cap);
      Revolver& setCapBack(bool cap);

      Revolver& setArc(float a1, float a2);
      Revolver& setSegmentLength(float length);

      /*
       * CONTOURS CAN BE EXPORTED ONLY FOLLOWING A process OPERATION
      */
      Revolver& setContourCapture(int capture);
      bool exportContours(IndexedVertexBatch<XYZ> &batch) const;

      template<int V = XYZ>
      bool process(IndexedVertexBatch<V> &batch, const Matrix &matrix, const std::vector<glm::vec2> &polyline);

    protected:
      GLenum frontFace = CCW;
      glm::vec2 textureOffset;
      float textureScale = 1;
      glm::vec4 color = { 1, 1, 1, 1 };

      bool capFront = false;
      bool capBack = false;

      float a1 = 0;
      float a2 = TWO_PI;
      float segmentLength = 8;

      float minRadius;
      float maxRadius;

      int contourCapture = CAPTURE_NONE;
      std::vector<Vertex<XYZ>> contourVertices;
      int contourRowCount, contourColumnCount;

      bool scanProfile(const std::vector<glm::vec2> &polyline);
      glm::vec2 getTextureCoords(const gl::Texture &texture, const glm::vec2 &xy) const;

      // ---

      template<int V>
      void updateIndices(IndexedVertexBatch<V> &batch, int columnCount, int rowCount, bool frontCapped, bool backCapped, bool weldedCaps)
      {
        int shift = 0;
        int frontCoreCapIndex, backCoreCapIndex;

        if (frontCapped)
        {
          frontCoreCapIndex = shift;
          shift++;
        }

        if (backCapped)
        {
          backCoreCapIndex = shift;
          shift++;
        }

        for (auto column = 0; column < columnCount - 1; column++)
        {
          for (auto row = 0; row < rowCount - 1; row++)
          {
            auto i0 = shift + column * rowCount + row;
            auto i1 = i0 + rowCount;

            auto i2 = i0 + 1;
            auto i3 = i2 + rowCount;

            if (frontFace == CCW)
            {
              batch.addIndices(i0, i2, i3, i3, i1, i0);
            }
            else
            {
              batch.addIndices(i0, i1, i3, i3, i2, i0);
            }
          }
        }

        int extraIndices = 0;

        if (weldedCaps)
        {
          updateWeldedCapIndices(batch, columnCount, rowCount, frontCapped, false, shift, frontCoreCapIndex);
          updateWeldedCapIndices(batch, columnCount, rowCount, false, backCapped, shift, backCoreCapIndex);
        }

        shift += columnCount * rowCount;

        // ---

        if (!weldedCaps)
        {
          if (frontCapped)
          {
            for (auto column = 0; column < columnCount - 1; column++)
            {
              auto i0 = shift + column;
              auto i1 = i0 + 1;

              if (frontFace == CCW)
              {
                batch.addIndices(frontCoreCapIndex, i0, i1);
              }
              else
              {
                batch.addIndices(i0, frontCoreCapIndex, i1);
              }
            }

            shift += columnCount;
          }

          if (backCapped)
          {
            for (auto column = 0; column < columnCount - 1; column++)
            {
              auto i0 = shift + column;
              auto i1 = i0 + 1;

              if (frontFace == CCW)
              {
                batch.addIndices(i0, backCoreCapIndex, i1);
              }
              else
              {
                batch.addIndices(backCoreCapIndex, i0, i1);
              }
            }

            shift += columnCount;
          }
        }

        // ---

        batch.incrementIndices(shift);
      }

      template<int V>
      void updateWeldedCapIndices(IndexedVertexBatch<V> &batch, int columnCount, int rowCount, bool frontCapped, bool backCapped, int shift, int coreCapIndex)
      {
        if (frontCapped || backCapped)
        {
          for (auto column = 0; column < columnCount - 1; column++)
          {
            auto row = rowCount - 1;

            auto i0 = shift + row + rowCount * column;
            auto i1 = shift + row + rowCount * ((column + 1) % (columnCount - 1));

            auto i2 = i0 + 1;
            auto i3 = i1 + 1;

            if (frontCapped)
            {
              if (frontFace == CCW)
              {
                batch.addIndices(coreCapIndex, i2, i3);
              }
              else
              {
                batch.addIndices(i2, coreCapIndex, i3);
              }
            }
            else if (backCapped)
            {
              if (frontFace == CCW)
              {
                batch.addIndices(i0, coreCapIndex, i1);
              }
              else
              {
                batch.addIndices(coreCapIndex, i0, i1);
              }
            }
          }
        }
      }

      // ---

      template<int V, typename... Args>
      void performProcess(IndexedVertexBatch<V> &batch, const Matrix &matrix, const std::vector<glm::vec2> &polyline, Args&&... args)
      {
        bool frontCapped = false;
        bool backCapped = false;

        if (capFront && (polyline.front().x != 0))
        {
          frontCapped = true;
          batch.addVertex(matrix.transformPoint(0, polyline.front().y, 0), std::forward<Args>(args)...);
        }

        if (capBack && (polyline.back().x != 0))
        {
          backCapped = true;
          batch.addVertex(matrix.transformPoint(0, polyline.back().y, 0), std::forward<Args>(args)...);
        }

        float aa = fabsf(a2 - a1);
        int columnCount = ceilf(aa * maxRadius / segmentLength) + 1;
        int rowCount = polyline.size();

        if (contourCapture)
        {
          contourRowCount = rowCount;
          contourColumnCount = columnCount;
          contourVertices.clear();
        }

        for (auto column = 0; column < columnCount; column++)
        {
          float d = a1 + fminf(aa, column * segmentLength / maxRadius);
          float sx = +sinf(d);
          float cz = +cosf(d);

          for (auto row = 0; row < rowCount; row++)
          {
            float r = polyline[row].x;
            float y = polyline[row].y;
            auto point = matrix.transformPoint(sx * r, y, cz * r);

            if (row == 0)
            {
              if (contourCapture & (CAPTURE_CAP_FRONT | CAPTURE_ROWS | CAPTURE_COLUMNS))
              {
                contourVertices.emplace_back(point);
              }
            }
            else if (row == rowCount - 1)
            {
              if (contourCapture & (CAPTURE_CAP_BACK | CAPTURE_ROWS | CAPTURE_COLUMNS))
              {
                contourVertices.emplace_back(point);
              }
            }
            else if (contourCapture & (CAPTURE_ROWS | CAPTURE_COLUMNS))
            {
              contourVertices.emplace_back(point);
            }

            batch.addVertex(point, std::forward<Args>(args)...);
          }
        }

        updateIndices(batch, columnCount, rowCount, frontCapped, backCapped, true);
      }

      template<int V, typename... Args>
      void performProcessWithNormals(IndexedVertexBatch<V> &batch, const Matrix &matrix, const std::vector<glm::vec2> &polyline, Args&&... args)
      {
        bool frontCapped = false;
        bool backCapped = false;

        glm::vec3 normalStart(0, -1, 0);
        glm::vec3 normalEnd(0, +1, 0);

        if (capFront && (polyline.front().x != 0))
        {
          frontCapped = true;
          batch.addVertex(
            matrix.transformPoint(0, polyline.front().y, 0),
            normalStart,
            std::forward<Args>(args)...);
        }

        if (capBack && (polyline.back().x != 0))
        {
          backCapped = true;
          batch.addVertex(
            matrix.transformPoint(0, polyline.back().y, 0),
            normalEnd,
            std::forward<Args>(args)...);
        }

        std::vector<glm::vec3> frontRowPoints;
        std::vector<glm::vec3> backRowPoints;

        float aa = fabsf(a2 - a1);
        int columnCount = ceilf(aa * maxRadius / segmentLength) + 1;
        int rowCount = polyline.size();

        if (contourCapture)
        {
          contourRowCount = rowCount;
          contourColumnCount = columnCount;
          contourVertices.clear();
        }

        for (auto column = 0; column < columnCount; column++)
        {
          float d = a1 + fminf(aa, column * segmentLength / maxRadius);
          float sx = +sinf(d);
          float cz = +cosf(d);

          for (auto row = 0; row < rowCount; row++)
          {
            float r = polyline[row].x;
            float y = polyline[row].y;
            auto point = matrix.transformPoint(sx * r, y, cz * r);

            if (row == 0)
            {
              if (frontCapped)
              {
                frontRowPoints.push_back(point);
              }

              if (contourCapture & (CAPTURE_CAP_FRONT | CAPTURE_ROWS | CAPTURE_COLUMNS))
              {
                contourVertices.emplace_back(point);
              }
            }
            else if (row == rowCount - 1)
            {
              if (backCapped)
              {
                backRowPoints.push_back(point);
              }

              if (contourCapture & (CAPTURE_CAP_BACK | CAPTURE_ROWS | CAPTURE_COLUMNS))
              {
                contourVertices.emplace_back(point);
              }
            }
            else if (contourCapture & (CAPTURE_ROWS | CAPTURE_COLUMNS))
            {
              contourVertices.emplace_back(point);
            }

            batch.addVertex(
              point,
              glm::vec3(sx, 0, cz),
              std::forward<Args>(args)...);
          }
        }

        if (frontCapped)
        {
          for (const auto &point : frontRowPoints)
          {
            batch.addVertex(
              point,
              normalStart,
              std::forward<Args>(args)...);
          }
        }

        if (backCapped)
        {
          for (const auto &point : backRowPoints)
          {
            batch.addVertex(
              point,
              normalEnd,
              std::forward<Args>(args)...);
          }
        }

        updateIndices(batch, columnCount, rowCount, frontCapped, backCapped, false);
      }

      template<int V, typename... Args>
      void performProcessWithTexture(IndexedVertexBatch<V> &batch, const Matrix &matrix, const std::vector<glm::vec2> &polyline, Args&&... args)
      {
        bool frontCapped = false;
        bool backCapped = false;

        if (capFront && (polyline.front().x != 0))
        {
          frontCapped = true;
          batch.addVertex(
            matrix.transformPoint(0, polyline.front().y, 0),
            glm::vec2(0, 0),
            std::forward<Args>(args)...);
        }

        if (capBack && (polyline.back().x != 0))
        {
          backCapped = true;
          batch.addVertex(
            matrix.transformPoint(0, polyline.back().y, 0),
            glm::vec2(0, 0),
            std::forward<Args>(args)...);
        }

        std::vector<Vertex<XYZ.UV>> frontRowVertices;
        std::vector<Vertex<XYZ.UV>> backRowVertices;

        float aa = fabsf(a2 - a1);
        int columnCount = ceilf(aa * maxRadius / segmentLength) + 1;
        int rowCount = polyline.size();

        if (contourCapture)
        {
          contourRowCount = rowCount;
          contourColumnCount = columnCount;
          contourVertices.clear();
        }

        float length = 0;
        std::vector<float> lengths;
        lengths.reserve(rowCount);

        for (auto column = 0; column < columnCount; column++)
        {
          float d = a1 + fminf(aa, column * segmentLength / maxRadius);
          float sx = +sinf(d);
          float cz = +cosf(d);

          for (auto row = 0; row < rowCount; row++)
          {
            float r = polyline[row].x;
            float y = polyline[row].y;
            auto point = matrix.transformPoint(sx * r, y, cz * r);

            if (column == 0)
            {
              if (row != 0)
              {
                length += glm::length(polyline[row] - polyline[row - 1]);
              }

              lengths[row] = length;
            }

            if (row == 0)
            {
              if (frontCapped)
              {
                frontRowVertices.emplace_back(point, sx * r, cz * r);
              }

              if (contourCapture & (CAPTURE_CAP_FRONT | CAPTURE_ROWS | CAPTURE_COLUMNS))
              {
                contourVertices.emplace_back(point);
              }
            }
            else if (row == rowCount - 1)
            {
              if (backCapped)
              {
                backRowVertices.emplace_back(point, sx * r, cz * r);
              }

              if (contourCapture & (CAPTURE_CAP_BACK | CAPTURE_ROWS | CAPTURE_COLUMNS))
              {
                contourVertices.emplace_back(point);
              }
            }
            else if (contourCapture & (CAPTURE_ROWS | CAPTURE_COLUMNS))
            {
              contourVertices.emplace_back(point);
            }

            batch.addVertex(
              point,
              getTextureCoords(batch.texture, glm::vec2(d * maxRadius, lengths[row])),
              std::forward<Args>(args)...);
          }
        }

        if (frontCapped)
        {
          for (const auto &vertex : frontRowVertices)
          {
            batch.addVertex(
              vertex.position,
              getTextureCoords(batch.texture, vertex.coords),
              std::forward<Args>(args)...);
          }
        }

        if (backCapped)
        {
          for (const auto &vertex : backRowVertices)
          {
            batch.addVertex(
              vertex.position,
              getTextureCoords(batch.texture, vertex.coords),
              std::forward<Args>(args)...);
          }
        }

        updateIndices(batch, columnCount, rowCount, frontCapped, backCapped, false);
      }

      template<int V, typename... Args>
      void performProcessWithNormalsAndTexture(IndexedVertexBatch<V> &batch, const Matrix &matrix, const std::vector<glm::vec2> &polyline, Args&&... args)
      {
        bool frontCapped = false;
        bool backCapped = false;

        glm::vec3 normalStart(0, -1, 0);
        glm::vec3 normalEnd(0, +1, 0);

        if (capFront && (polyline.front().x != 0))
        {
          frontCapped = true;
          batch.addVertex(
            matrix.transformPoint(0, polyline.front().y, 0),
            normalStart,
            glm::vec2(0, 0),
            std::forward<Args>(args)...);
        }

        if (capBack && (polyline.back().x != 0))
        {
          backCapped = true;
          batch.addVertex(
            matrix.transformPoint(0, polyline.back().y, 0),
            normalEnd,
            glm::vec2(0, 0),
            std::forward<Args>(args)...);
        }

        std::vector<Vertex<XYZ.UV>> frontRowVertices;
        std::vector<Vertex<XYZ.UV>> backRowVertices;

        float aa = fabsf(a2 - a1);
        int columnCount = ceilf(aa * maxRadius / segmentLength) + 1;
        int rowCount = polyline.size();

        if (contourCapture)
        {
          contourRowCount = rowCount;
          contourColumnCount = columnCount;
          contourVertices.clear();
        }

        float length = 0;
        std::vector<float> lengths;
        lengths.reserve(rowCount);

        for (auto column = 0; column < columnCount; column++)
        {
          float d = a1 + fminf(aa, column * segmentLength / maxRadius);
          float sx = +sinf(d);
          float cz = +cosf(d);

          for (auto row = 0; row < rowCount; row++)
          {
            float r = polyline[row].x;
            float y = polyline[row].y;
            auto point = matrix.transformPoint(sx * r, y, cz * r);

            if (column == 0)
            {
              if (row != 0)
              {
                length += glm::length(polyline[row] - polyline[row - 1]);
              }

              lengths[row] = length;
            }

            if (row == 0)
            {
              if (frontCapped)
              {
                frontRowVertices.emplace_back(point, sx * r, cz * r);
              }

              if (contourCapture & (CAPTURE_CAP_FRONT | CAPTURE_ROWS | CAPTURE_COLUMNS))
              {
                contourVertices.emplace_back(point);
              }
            }
            else if (row == rowCount - 1)
            {
              if (backCapped)
              {
                backRowVertices.emplace_back(point, sx * r, cz * r);
              }

              if (contourCapture & (CAPTURE_CAP_BACK | CAPTURE_ROWS | CAPTURE_COLUMNS))
              {
                contourVertices.emplace_back(point);
              }
            }
            else if (contourCapture & (CAPTURE_ROWS | CAPTURE_COLUMNS))
            {
              contourVertices.emplace_back(point);
            }

            batch.addVertex(
              point,
              glm::vec3(sx, 0, cz),
              getTextureCoords(batch.texture, glm::vec2(d * maxRadius, lengths[row])),
              std::forward<Args>(args)...);
          }
        }

        if (frontCapped)
        {
          for (const auto &vertex : frontRowVertices)
          {
            batch.addVertex(
              vertex.position,
              normalStart,
              getTextureCoords(batch.texture, vertex.coords),
              std::forward<Args>(args)...);
          }
        }

        if (backCapped)
        {
          for (const auto &vertex : backRowVertices)
          {
            batch.addVertex(
              vertex.position,
              normalEnd,
              getTextureCoords(batch.texture, vertex.coords),
              std::forward<Args>(args)...);
          }
        }

        updateIndices(batch, columnCount, rowCount, frontCapped, backCapped, false);
      }
    };
  }
}
