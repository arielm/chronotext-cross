#include "gl/Triangulator.h"

using namespace std;
using namespace chr;
using namespace math;
using namespace path;

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

    Triangulator& Triangulator::setWindingRule(TessWindingRule windingRule)
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

    Triangulator& Triangulator::setContourCapture(CaptureType capture)
    {
      contourCapture = capture;
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

          auto size = contour.size();

          for (size_t i = 0; i < size; i++)
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

            auto size = contour.size();

            for (size_t i = 0; i < size; i++)
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

            auto size2 = contour.size() * 2;

            for (auto i = 0; i < size2; i++)
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

    Triangulator& Triangulator::add(const Shape &shape)
    {
      windingRule = convert(shape.getFillRule());

      for (auto &path : shape.getPaths())
      {
        add(path.getPolyline());
      }

      return *this;
    }

    Triangulator& Triangulator::add(const vector<glm::vec2> &polyline)
    {
      tessAddContour(tess, 2, polyline.data(), sizeof(glm::vec2), polyline.size());
      return *this;
    }

    // ---

    template <>
    void Triangulator::fill(IndexedVertexBatch<XYZ> &batch)
    {
      performFill(batch);
    }

    template <>
    void Triangulator::fill(IndexedVertexBatch<XYZ.RGBA> &batch)
    {
      performFill(batch, color);
    }

    template <>
    void Triangulator::fill(IndexedVertexBatch<XYZ.UV> &batch)
    {
      performFillWithTexture(batch);
    }

    template <>
    void Triangulator::fill(IndexedVertexBatch<XYZ.UV.RGBA> &batch)
    {
      performFillWithTexture(batch, color);
    }

    // ---

    template <>
    void Triangulator::fill(IndexedVertexBatch<XYZ> &batch, Matrix &matrix)
    {
      performFill(batch, matrix);
    }

    template <>
    void Triangulator::fill(IndexedVertexBatch<XYZ.N> &batch, Matrix &matrix)
    {
      auto normal = matrix.transformNormal(0, 0, 1);
      performFill(batch, matrix, normal);
    }

    template <>
    void Triangulator::fill(IndexedVertexBatch<XYZ.N.UV> &batch, Matrix &matrix)
    {
      auto normal = matrix.transformNormal(0, 0, 1);
      performFillWithNormalsAndTexture(batch, matrix, normal);
    }

    template <>
    void Triangulator::fill(IndexedVertexBatch<XYZ.UV> &batch, Matrix &matrix)
    {
      performFillWithTexture(batch, matrix);
    }

    template <>
    void Triangulator::fill(IndexedVertexBatch<XYZ.RGBA> &batch, Matrix &matrix)
    {
      performFill(batch, matrix, color);
    }

    template <>
    void Triangulator::fill(IndexedVertexBatch<XYZ.N.RGBA> &batch, Matrix &matrix)
    {
      auto normal = matrix.transformNormal(0, 0, 1);
      performFill(batch, matrix, normal, color);
    }

    template <>
    void Triangulator::fill(IndexedVertexBatch<XYZ.UV.RGBA> &batch, Matrix &matrix)
    {
      performFillWithTexture(batch, matrix, color);
    }

    template <>
    void Triangulator::fill(IndexedVertexBatch<XYZ.N.UV.RGBA> &batch, Matrix &matrix)
    {
      auto normal = matrix.transformNormal(0, 0, 1);
      performFillWithNormalsAndTexture(batch, matrix, normal, color);
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

    TessWindingRule Triangulator::convert(Shape::FillRule fillRule)
    {
      switch (fillRule)
      {
        default:
        case Shape::FILLRULE_EVENODD:
          return TESS_WINDING_ODD;

        case Shape::FILLRULE_NONZERO:
          return TESS_WINDING_NONZERO;
      }
    }
  }
}
