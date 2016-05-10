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

    Triangulator& setColor(const glm::vec4 &color);
    Triangulator& setColor(float r, float g, float b, float a);

    template<int Orientation = GL_CCW>
    Triangulator& add(const math::Rectf &rect);

    Triangulator& add(const std::vector<std::vector<glm::vec2>> &polygons);
    Triangulator& add(const std::vector<glm::vec2> &polygon);

    template<int V = gl::XYZ>
    void stamp(gl::IndexedVertexBatch<V> &batch, gl::Matrix &matrix);

    template<int V = gl::XYZ>
    void extrude(gl::IndexedVertexBatch<V> &batch, gl::Matrix &matrix, float distance);

  protected:
    TESStesselator *tess;
    int allocated = 0;

    int windingRule = TESS_WINDING_ODD;
    GLenum frontFace = GL_CCW;
    glm::vec4 color;

    template<int V = gl::XYZ, typename... Args>
    void performStamp(gl::IndexedVertexBatch<V> &batch, gl::Matrix &matrix, Args&&... args)
    {
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

    template<int V = gl::XYZ, typename... Args>
    void performExtrude(gl::IndexedVertexBatch<V> &batch, gl::Matrix &matrix, float distance, Args&&... args)
    {
      bool sign = (distance > 0);

      tessTesselate(tess, windingRule, TESS_BOUNDARY_CONTOURS, 0, 0, 0);

      auto vertices = (glm::vec2*)tessGetVertices(tess);
      auto elements = tessGetElements(tess);
      auto elementCount = tessGetElementCount(tess);

      for (auto i = 0; i < elementCount; i++)
      {
        const auto base = elements[i << 1];
        const auto count = elements[(i << 1) + 1];

        for (int j = 0; j < count; j++)
        {
          auto &p0 = vertices[base + j];
          auto &p1 = vertices[base + (j + 1) % count];

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

    template<int V = gl::XYZ, typename... Args>
    void performExtrudeWithNormals(gl::IndexedVertexBatch<V> &batch, gl::Matrix &matrix, float distance, Args&&... args)
    {
      bool sign = (distance > 0);

      tessTesselate(tess, windingRule, TESS_BOUNDARY_CONTOURS, 0, 0, 0);

      auto vertices = (glm::vec2*)tessGetVertices(tess);
      auto elements = tessGetElements(tess);
      auto elementCount = tessGetElementCount(tess);

      for (auto i = 0; i < elementCount; i++)
      {
        const auto base = elements[i << 1];
        const auto count = elements[(i << 1) + 1];

        for (int j = 0; j < count; j++)
        {
          auto &p0 = vertices[base + j];
          auto &p1 = vertices[base + (j + 1) % count];
          auto tangeant = glm::normalize(p1 - p0).yx() * glm::vec2(-1, +1);

          auto normal = matrix.transformNormal(glm::vec3(tangeant, 0));

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
