#include "gl/Triangulator.h"

using namespace std;
using namespace chr::math;

namespace chr
{
  namespace gl
  {
    static void* stdAlloc(void* userData, unsigned int size)
    {
      int* allocated = (int*)userData;
      *allocated += (int)size;
      return malloc(size);
    }

    static void stdFree(void* userData, void* ptr)
    {
      free(ptr);
    }

    Triangulator::Triangulator()
    {
      TESSalloc ma;
      memset(&ma, 0, sizeof(ma));
      ma.memalloc = stdAlloc;
      ma.memfree = stdFree;
      ma.userData = (void*)&allocated;
      ma.extraVertices = 256; // realloc not provided, allow 256 extra vertices.

      tess = tessNewTess(&ma);
    }

    Triangulator::~Triangulator()
    {
      tessDeleteTess(tess);
    }

    Triangulator& Triangulator::setWindingRule(int windingRule)
    {
      this->windingRule = windingRule;
      return *this;
    }

    Triangulator& Triangulator::setFrontFace(GLenum mode)
    {
      frontFace = mode;
      return *this;
    }

    Triangulator& Triangulator::setTextureOffset(const glm::vec2 &offset)
    {
      textureOffset = offset;
      return *this;
    }

    Triangulator& Triangulator::setTextureOffset(float x, float y)
    {
      textureOffset = glm::vec2(x, y);
      return *this;
    }

    Triangulator& Triangulator::setTextureScale(float scale)
    {
      textureScale = scale;
      return *this;
    }

    Triangulator& Triangulator::setColor(const glm::vec4 &color)
    {
      this->color = color;
      return *this;
    }

    Triangulator& Triangulator::setColor(float r, float g, float b, float a)
    {
      color = { r, g, b, a };
      return *this;
    }

    // ---

    Triangulator& Triangulator::setContourCapture(int contourCapture)
    {
      this->contourCapture = contourCapture;
      return *this;
    }

    void Triangulator::exportContours(IndexedVertexBatch<XYZ> &batch, Matrix &matrix) const
    {
      if (contourCapture & CAPTURE_FRONT)
      {
        for (const auto &contour : contours)
        {
          for (const auto &point : contour)
          {
            batch.addVertex(matrix.transformPoint(point));
          }

          int size = contour.size();

          for (int i = 0; i < size; i++)
          {
            batch.addIndices(i, (i + 1) % size);
          }

          batch.incrementIndices(size);
        }
      }

      if (extrudedDistance != 0)
      {
        if (contourCapture & CAPTURE_BACK)
        {
          for (const auto &contour : contours)
          {
            for (const auto &point : contour)
            {
              batch.addVertex(matrix.transformPoint(glm::vec3(point, extrudedDistance)));
            }

            int size = contour.size();

            for (int i = 0; i < size; i++)
            {
              batch.addIndices(i, (i + 1) % size);
            }

            batch.incrementIndices(size);
          }
        }

        if (contourCapture & CAPTURE_HEIGHT)
        {
          for (const auto &contour : contours)
          {
            for (const auto &point : contour)
            {
              batch.addVertex(matrix.transformPoint(point));
              batch.addVertex(matrix.transformPoint(glm::vec3(point, extrudedDistance)));
            }

            int size2 = contour.size() * 2;

            for (int i = 0; i < size2; i++)
            {
              batch.addIndices(i);
            }

            batch.incrementIndices(size2);
          }
        }
      }
    }

    void Triangulator::captureContours()
    {
      tessTesselate(tess, windingRule, TESS_BOUNDARY_CONTOURS, 0, 0, 0);

      auto vertices = (glm::vec2*)tessGetVertices(tess);
      auto elements = tessGetElements(tess);
      auto elementCount = tessGetElementCount(tess);

      contours.clear();
      contours.reserve(elementCount);

      for (auto i = 0; i < elementCount; i++)
      {
        const auto base = elements[i << 1];
        const auto count = elements[(i << 1) + 1];

        contours.emplace_back();
        contours.back().reserve(count);

        for (int j = 0; j < count; j++)
        {
          contours.back().emplace_back(vertices[base + j]);
        }

        /*
         * NECESSARY TO ADD THE CONTOURS BACK FOR FURTHER tessTesselate OPERATIONS
         */
        tessAddContour(tess, 2, &vertices[base], sizeof(glm::vec2), count);
      }
    }

    // ---

    template <>
    Triangulator& Triangulator::add<GL_CCW>(const Rectf &rect)
    {
      vector<glm::vec2> polygon;
      polygon.emplace_back(rect.x1y1());
      polygon.emplace_back(rect.x1y2());
      polygon.emplace_back(rect.x2y2());
      polygon.emplace_back(rect.x2y1());

      add(polygon);
      return *this;
    }

    template <>
    Triangulator& Triangulator::add<GL_CW>(const Rectf &rect)
    {
      vector<glm::vec2> polygon;
      polygon.emplace_back(rect.x1y1());
      polygon.emplace_back(rect.x2y1());
      polygon.emplace_back(rect.x2y2());
      polygon.emplace_back(rect.x1y2());

      add(polygon);
      return *this;
    }

    Triangulator& Triangulator::add(const vector<vector<glm::vec2>> &polygons)
    {
      for (auto &polygon : polygons)
      {
        add(polygon);
      }

      return *this;
    }

    Triangulator& Triangulator::add(const vector<glm::vec2> &polygon)
    {
      tessAddContour(tess, 2, polygon.data(), sizeof(glm::vec2), polygon.size());
      return *this;
    }

    // ---

    template <>
    void Triangulator::stamp(IndexedVertexBatch<XYZ> &batch, Matrix &matrix)
    {
      performStamp(batch, matrix);
    }

    template <>
    void Triangulator::stamp(IndexedVertexBatch<XYZ.N> &batch, Matrix &matrix)
    {
      auto normal = matrix.transformNormal(0, 0, 1);
      performStamp(batch, matrix, normal);
    }

    template <>
    void Triangulator::stamp(IndexedVertexBatch<XYZ.N.UV> &batch, Matrix &matrix)
    {
      auto normal = matrix.transformNormal(0, 0, 1);
      performStampWithNormalsAndTexture(batch, matrix, normal);
    }

    template <>
    void Triangulator::stamp(IndexedVertexBatch<XYZ.UV> &batch, Matrix &matrix)
    {
      performStampWithTexture(batch, matrix);
    }

    template <>
    void Triangulator::stamp(IndexedVertexBatch<XYZ.RGBA> &batch, Matrix &matrix)
    {
      performStamp(batch, matrix, color);
    }

    template <>
    void Triangulator::stamp(IndexedVertexBatch<XYZ.N.RGBA> &batch, Matrix &matrix)
    {
      auto normal = matrix.transformNormal(0, 0, 1);
      performStamp(batch, matrix, normal, color);
    }

    template <>
    void Triangulator::stamp(IndexedVertexBatch<XYZ.UV.RGBA> &batch, Matrix &matrix)
    {
      performStampWithTexture(batch, matrix, color);
    }

    template <>
    void Triangulator::stamp(IndexedVertexBatch<XYZ.N.UV.RGBA> &batch, Matrix &matrix)
    {
      auto normal = matrix.transformNormal(0, 0, 1);
      performStampWithNormalsAndTexture(batch, matrix, normal, color);
    }

    // ---

    template <>
    void Triangulator::extrude(IndexedVertexBatch<XYZ> &batch, Matrix &matrix, float distance)
    {
      if (distance != 0)
      {
        performExtrude(batch, matrix, distance);
      }
    }

    template <>
    void Triangulator::extrude(IndexedVertexBatch<XYZ.N> &batch, Matrix &matrix, float distance)
    {
      if (distance != 0)
      {
        performExtrudeWithNormals(batch, matrix, distance);
      }
    }

    template <>
    void Triangulator::extrude(IndexedVertexBatch<XYZ.UV> &batch, Matrix &matrix, float distance)
    {
      if (distance != 0)
      {
        performExtrudeWithTexture(batch, matrix, distance);
      }
    }

    template <>
    void Triangulator::extrude(IndexedVertexBatch<XYZ.N.UV> &batch, Matrix &matrix, float distance)
    {
      if (distance != 0)
      {
        performExtrudeWithNormalsAndTexture(batch, matrix, distance);
      }
    }

    template <>
    void Triangulator::extrude(IndexedVertexBatch<XYZ.RGBA> &batch, Matrix &matrix, float distance)
    {
      if (distance != 0)
      {
        performExtrude(batch, matrix, distance, color);
      }
    }

    template <>
    void Triangulator::extrude(IndexedVertexBatch<XYZ.UV.RGBA> &batch, Matrix &matrix, float distance)
    {
      if (distance != 0)
      {
        performExtrudeWithTexture(batch, matrix, distance, color);
      }
    }

    template <>
    void Triangulator::extrude(IndexedVertexBatch<XYZ.N.RGBA> &batch, Matrix &matrix, float distance)
    {
      if (distance != 0)
      {
        performExtrudeWithNormals(batch, matrix, distance, color);
      }
    }

    template <>
    void Triangulator::extrude(IndexedVertexBatch<XYZ.N.UV.RGBA> &batch, Matrix &matrix, float distance)
    {
      if (distance != 0)
      {
        performExtrudeWithNormalsAndTexture(batch, matrix, distance, color);
      }
    }

    // ---

    glm::vec2 Triangulator::getTextureCoords(const Texture &texture, const glm::vec2 &xy) const
    {
      return (xy - textureOffset) / (texture.innerSize * textureScale);
    }
  }
}