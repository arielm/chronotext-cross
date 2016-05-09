#include "Triangulator.h"

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

  Triangulator& Triangulator::add(const vector<std::vector<glm::vec2>> &polygons)
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

  Triangulator& Triangulator::process(IndexedVertexBatch<XYZ> &batch, Matrix &matrix, int windingRule)
  {
    tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

    // ---

    auto vertices = (glm::vec2*)tessGetVertices(tess);
    auto vertexCount = tessGetVertexCount(tess);

    auto &batchVertices = batch.vertexBuffer->storage;
    batchVertices.reserve(batchVertices.size() + vertexCount);

    for (int i = 0; i < vertexCount; i++)
    {
      batchVertices.emplace_back(matrix.transformPoint(vertices[i]));
    }

    // ---

    auto indices = (int*)tessGetElements(tess);
    auto indexCount =  tessGetElementCount(tess) * 3;

    auto &batchIndices = batch.indexBuffer->storage;
    batchIndices.reserve(batchIndices.size() + indexCount);

    for (int i = 0; i < indexCount; i++)
    {
      batchIndices.emplace_back(indices[i]);
    }

    return *this;
  }

  Triangulator& Triangulator::process(IndexedVertexBatch<XYZ.N.RGBA> &batch, Matrix &matrix, const glm::vec4 &color, int windingRule)
  {
    tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

    // ---

    auto vertices = (glm::vec2*)tessGetVertices(tess);
    auto vertexCount = tessGetVertexCount(tess);

    auto &batchVertices = batch.vertexBuffer->storage;
    batchVertices.reserve(batchVertices.size() + vertexCount);

    auto normal = matrix.transformNormal(0, 0, 1);

    for (int i = 0; i < vertexCount; i++)
    {
      batchVertices.emplace_back(matrix.transformPoint(vertices[i]), normal, color);
    }

    // ---

    auto indices = (int*)tessGetElements(tess);
    auto indexCount =  tessGetElementCount(tess) * 3;

    auto &batchIndices = batch.indexBuffer->storage;
    batchIndices.reserve(batchIndices.size() + indexCount);

    for (int i = 0; i < indexCount; i++)
    {
      batchIndices.emplace_back(indices[i]);
    }

    return *this;
  }
}
