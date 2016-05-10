#include "Triangulator.h"

#include "Log.h"

using namespace std;
using namespace chr::gl;
using namespace chr::math;

namespace chr
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

  template <>
  void Triangulator::stamp(IndexedVertexBatch<XYZ> &batch, Matrix &matrix)
  {
    stamp(batch, matrix);
  }

  template <>
  void Triangulator::stamp(IndexedVertexBatch<XYZ.N> &batch, Matrix &matrix)
  {
    auto normal = matrix.transformNormal(0, 0, (frontFace == GL_CW) ? -1 : +1);
    stamp(batch, matrix, normal);
  }

  template <>
  void Triangulator::stamp(IndexedVertexBatch<XYZ.RGBA> &batch, Matrix &matrix)
  {
    stamp(batch, matrix, color);
  }

  template <>
  void Triangulator::stamp(IndexedVertexBatch<XYZ.N.RGBA> &batch, Matrix &matrix)
  {
    auto normal = matrix.transformNormal(0, 0, (frontFace == GL_CW) ? -1 : +1);
    stamp(batch, matrix, normal, color);
  }

  template <>
  void Triangulator::extrude(IndexedVertexBatch<XYZ.N.RGBA> &batch, Matrix &matrix, float distance)
  {
    if (distance != 0)
    {
      bool sign = (distance > 0);

      auto contours = getContourSegments();

      for (auto &contour : contours)
      {
        for (auto &segment : contour)
        {
          auto normal = matrix.transformNormal(glm::vec3(segment.tangeant, 0));

          batch
            .addVertex(matrix.transformPoint(segment.p1), normal, color)
            .addVertex(matrix.transformPoint(segment.p2), normal, color)
            .addVertex(matrix.transformPoint(glm::vec3(segment.p2, distance)), normal, color)
            .addVertex(matrix.transformPoint(glm::vec3(segment.p1, distance)), normal, color);

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
      }

      // ---

      tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

      auto vertices = (glm::vec2 *) tessGetVertices(tess);
      auto vertexCount = tessGetVertexCount(tess);

      auto indices = (int *) tessGetElements(tess);
      auto indexCount = tessGetElementCount(tess) * 3;

      // ---

      auto normal1 = matrix.transformNormal(0, 0, sign ? -1 : +1);

      for (int i = 0; i < vertexCount; i++)
      {
        batch.addVertex(matrix.transformPoint(vertices[i]), normal1, color);
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
        batch.addVertex(matrix.transformPoint(glm::vec3(vertices[i], distance)), normal2, color);
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
  }

  vector<vector<Triangulator::Segment>> Triangulator::getContourSegments() const
  {
    vector<vector<Segment>> contours;

    tessTesselate(tess, windingRule, TESS_BOUNDARY_CONTOURS, 0, 0, 0);

    const auto vertices = (glm::vec2*)tessGetVertices(tess);
    const auto elements = tessGetElements(tess);
    auto elementCount = tessGetElementCount(tess);

    for (auto i = 0; i < elementCount; i++)
    {
      const auto base = elements[i << 1];
      const auto count = elements[(i << 1) + 1];

      contours.emplace_back();

      for (int j = 0; j < count; j++)
      {
        auto &p0 = vertices[base + j];
        auto &p1 = vertices[base + (j + 1) % count];
        auto tangeant = glm::normalize(p1 - p0).yx() * glm::vec2(-1, +1);

        contours.back().emplace_back(p0, p1, tangeant);
      }

      /*
       * NECESSARY TO ADD THE CONTOURS BACK FOR FURTHER tessTesselate OPERATIONS
       */
      tessAddContour(tess, 2, &vertices[base], sizeof(glm::vec2), count);
    }

    return contours;
  }
}
