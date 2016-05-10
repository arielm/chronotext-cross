#pragma once

#include "math/Rect.h"
#include "gl/Batch.h"
#include "gl/Matrix.h"

#include "tesselator.h"

namespace chr
{
  class Triangulator
  {
  public:
    Triangulator();
    ~Triangulator();

    Triangulator& setWindingRule(int windingRule);

    Triangulator& setColor(const glm::vec4 &color);
    Triangulator& setColor(float r, float g, float b, float a);

    template<int Orientation = GL_CCW>
    Triangulator& add(const math::Rectf &rect);

    Triangulator& add(const std::vector<std::vector<glm::vec2>> &polygons);
    Triangulator& add(const std::vector<glm::vec2> &polygon);

    template<int V = gl::XYZ>
    void process(gl::IndexedVertexBatch<V> &batch, gl::Matrix &matrix);

  protected:
    TESStesselator *tess;
    int allocated = 0;

    int windingRule = TESS_WINDING_ODD;
    glm::vec4 color;

    template<int V = gl::XYZ, typename... Args>
    void process(gl::IndexedVertexBatch<V> &batch, gl::Matrix &matrix, Args&&... args)
    {
      tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);

      // ---

      auto vertices = (glm::vec2*)tessGetVertices(tess);
      auto vertexCount = tessGetVertexCount(tess);

      auto &batchVertices = batch.vertexBuffer->storage;
      batchVertices.reserve(batchVertices.size() + vertexCount);

      for (int i = 0; i < vertexCount; i++)
      {
        batchVertices.emplace_back(matrix.transformPoint(vertices[i]), std::forward<Args>(args)...);
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
    };
  };
}
