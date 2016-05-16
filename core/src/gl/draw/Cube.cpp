#include "gl/draw/Cube.h"

using namespace chr::math;

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      Cube& Cube::setSize(const glm::vec3 &size)
      {
        this->size = size;
        return *this;
      }

      Cube& Cube::setColor(const glm::vec4 &color)
      {
        this->color = color;
        return *this;
      }

      Cube& Cube::setColor(float r, float g, float b, float a)
      {
        color = { r, g, b, a };
        return *this;
      }

      template <>
      void Cube::append(IndexedVertexBatch<XYZ, GLushort> &batch, Matrix &matrix) const
      {
        // FRONT
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, +size.z))
          .addVertex(matrix.transformPoint(+size.x, -size.y, +size.z))
          .addVertex(matrix.transformPoint(+size.x, +size.y, +size.z))
          .addVertex(matrix.transformPoint(-size.x, +size.y, +size.z));

        // BACK
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, -size.z))
          .addVertex(matrix.transformPoint(-size.x, +size.y, -size.z))
          .addVertex(matrix.transformPoint(+size.x, +size.y, -size.z))
          .addVertex(matrix.transformPoint(+size.x, -size.y, -size.z));

        // TOP
        batch
          .addVertex(matrix.transformPoint(-size.x, +size.y, -size.z))
          .addVertex(matrix.transformPoint(-size.x, +size.y, +size.z))
          .addVertex(matrix.transformPoint(+size.x, +size.y, +size.z))
          .addVertex(matrix.transformPoint(+size.x, +size.y, -size.z));

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, -size.z))
          .addVertex(matrix.transformPoint(+size.x, -size.y, -size.z))
          .addVertex(matrix.transformPoint(+size.x, -size.y, +size.z))
          .addVertex(matrix.transformPoint(-size.x, -size.y, +size.z));

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(+size.x, -size.y, -size.z))
          .addVertex(matrix.transformPoint(+size.x, +size.y, -size.z))
          .addVertex(matrix.transformPoint(+size.x, +size.y, +size.z))
          .addVertex(matrix.transformPoint(+size.x, -size.y, +size.z));

        // LEFT
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, -size.z))
          .addVertex(matrix.transformPoint(-size.x, -size.y, +size.z))
          .addVertex(matrix.transformPoint(-size.x, +size.y, +size.z))
          .addVertex(matrix.transformPoint(-size.x, +size.y, -size.z));

        // ---

        batch
          .addIndices( 0,  2,  1,  0,  3,  2) /* FRONT */
          .addIndices( 4,  6,  5,  4,  7,  6) /* BACK */
          .addIndices( 8, 10,  9,  8, 11, 10) /* TOP */
          .addIndices(12, 14, 13, 12, 15, 14) /* BOTTOM */
          .addIndices(16, 18, 17, 16, 19, 18) /* RIGHT */
          .addIndices(20, 22, 21, 20, 23, 22) /* LEFT */
          .incrementIndices(24);
      }

      template <>
      void Cube::append(IndexedVertexBatch<XYZ.N, GLushort> &batch, Matrix &matrix) const
      {
        auto normal0 = matrix.transformNormal( 0,  0, +1);
        auto normal1 = matrix.transformNormal( 0,  0, -1);
        auto normal2 = matrix.transformNormal( 0, +1,  0);
        auto normal3 = matrix.transformNormal( 0, -1,  0);
        auto normal4 = matrix.transformNormal(+1,  0,  0);
        auto normal5 = matrix.transformNormal(-1,  0,  0);

        // FRONT
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, +size.z), normal0)
          .addVertex(matrix.transformPoint(+size.x, -size.y, +size.z), normal0)
          .addVertex(matrix.transformPoint(+size.x, +size.y, +size.z), normal0)
          .addVertex(matrix.transformPoint(-size.x, +size.y, +size.z), normal0);

        // BACK
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, -size.z), normal1)
          .addVertex(matrix.transformPoint(-size.x, +size.y, -size.z), normal1)
          .addVertex(matrix.transformPoint(+size.x, +size.y, -size.z), normal1)
          .addVertex(matrix.transformPoint(+size.x, -size.y, -size.z), normal1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(-size.x, +size.y, -size.z), normal2)
          .addVertex(matrix.transformPoint(-size.x, +size.y, +size.z), normal2)
          .addVertex(matrix.transformPoint(+size.x, +size.y, +size.z), normal2)
          .addVertex(matrix.transformPoint(+size.x, +size.y, -size.z), normal2);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, -size.z), normal3)
          .addVertex(matrix.transformPoint(+size.x, -size.y, -size.z), normal3)
          .addVertex(matrix.transformPoint(+size.x, -size.y, +size.z), normal3)
          .addVertex(matrix.transformPoint(-size.x, -size.y, +size.z), normal3);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(+size.x, -size.y, -size.z), normal4)
          .addVertex(matrix.transformPoint(+size.x, +size.y, -size.z), normal4)
          .addVertex(matrix.transformPoint(+size.x, +size.y, +size.z), normal4)
          .addVertex(matrix.transformPoint(+size.x, -size.y, +size.z), normal4);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, -size.z), normal5)
          .addVertex(matrix.transformPoint(-size.x, -size.y, +size.z), normal5)
          .addVertex(matrix.transformPoint(-size.x, +size.y, +size.z), normal5)
          .addVertex(matrix.transformPoint(-size.x, +size.y, -size.z), normal5);

        // ---

        batch
          .addIndices( 0,  2,  1,  0,  3,  2) /* FRONT */
          .addIndices( 4,  6,  5,  4,  7,  6) /* BACK */
          .addIndices( 8, 10,  9,  8, 11, 10) /* TOP */
          .addIndices(12, 14, 13, 12, 15, 14) /* BOTTOM */
          .addIndices(16, 18, 17, 16, 19, 18) /* RIGHT */
          .addIndices(20, 22, 21, 20, 23, 22) /* LEFT */
          .incrementIndices(24);
      }

      template <>
      void Cube::append(IndexedVertexBatch<XYZ.RGBA, GLushort> &batch, Matrix &matrix) const
      {
        auto color0 = getFaceColor(0);
        auto color1 = getFaceColor(1);
        auto color2 = getFaceColor(2);
        auto color3 = getFaceColor(3);
        auto color4 = getFaceColor(4);
        auto color5 = getFaceColor(5);

        // FRONT
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, +size.z), color0)
          .addVertex(matrix.transformPoint(+size.x, -size.y, +size.z), color0)
          .addVertex(matrix.transformPoint(+size.x, +size.y, +size.z), color0)
          .addVertex(matrix.transformPoint(-size.x, +size.y, +size.z), color0);

        // BACK
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, -size.z), color1)
          .addVertex(matrix.transformPoint(-size.x, +size.y, -size.z), color1)
          .addVertex(matrix.transformPoint(+size.x, +size.y, -size.z), color1)
          .addVertex(matrix.transformPoint(+size.x, -size.y, -size.z), color1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(-size.x, +size.y, -size.z), color2)
          .addVertex(matrix.transformPoint(-size.x, +size.y, +size.z), color2)
          .addVertex(matrix.transformPoint(+size.x, +size.y, +size.z), color2)
          .addVertex(matrix.transformPoint(+size.x, +size.y, -size.z), color2);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, -size.z), color3)
          .addVertex(matrix.transformPoint(+size.x, -size.y, -size.z), color3)
          .addVertex(matrix.transformPoint(+size.x, -size.y, +size.z), color3)
          .addVertex(matrix.transformPoint(-size.x, -size.y, +size.z), color3);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(+size.x, -size.y, -size.z), color4)
          .addVertex(matrix.transformPoint(+size.x, +size.y, -size.z), color4)
          .addVertex(matrix.transformPoint(+size.x, +size.y, +size.z), color4)
          .addVertex(matrix.transformPoint(+size.x, -size.y, +size.z), color4);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, -size.z), color5)
          .addVertex(matrix.transformPoint(-size.x, -size.y, +size.z), color5)
          .addVertex(matrix.transformPoint(-size.x, +size.y, +size.z), color5)
          .addVertex(matrix.transformPoint(-size.x, +size.y, -size.z), color5);

        // ---

        batch
          .addIndices( 0,  2,  1,  0,  3,  2) /* FRONT */
          .addIndices( 4,  6,  5,  4,  7,  6) /* BACK */
          .addIndices( 8, 10,  9,  8, 11, 10) /* TOP */
          .addIndices(12, 14, 13, 12, 15, 14) /* BOTTOM */
          .addIndices(16, 18, 17, 16, 19, 18) /* RIGHT */
          .addIndices(20, 22, 21, 20, 23, 22) /* LEFT */
          .incrementIndices(24);
      }

      template <>
      void Cube::append(IndexedVertexBatch<XYZ.N.RGBA, GLushort> &batch, Matrix &matrix) const
      {
        auto normal0 = matrix.transformNormal( 0,  0, +1);
        auto normal1 = matrix.transformNormal( 0,  0, -1);
        auto normal2 = matrix.transformNormal( 0, +1,  0);
        auto normal3 = matrix.transformNormal( 0, -1,  0);
        auto normal4 = matrix.transformNormal(+1,  0,  0);
        auto normal5 = matrix.transformNormal(-1,  0,  0);

        auto color0 = getFaceColor(0);
        auto color1 = getFaceColor(1);
        auto color2 = getFaceColor(2);
        auto color3 = getFaceColor(3);
        auto color4 = getFaceColor(4);
        auto color5 = getFaceColor(5);

        // FRONT
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, +size.z), normal0, color0)
          .addVertex(matrix.transformPoint(+size.x, -size.y, +size.z), normal0, color0)
          .addVertex(matrix.transformPoint(+size.x, +size.y, +size.z), normal0, color0)
          .addVertex(matrix.transformPoint(-size.x, +size.y, +size.z), normal0, color0);

        // BACK
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, -size.z), normal1, color1)
          .addVertex(matrix.transformPoint(-size.x, +size.y, -size.z), normal1, color1)
          .addVertex(matrix.transformPoint(+size.x, +size.y, -size.z), normal1, color1)
          .addVertex(matrix.transformPoint(+size.x, -size.y, -size.z), normal1, color1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(-size.x, +size.y, -size.z), normal2, color2)
          .addVertex(matrix.transformPoint(-size.x, +size.y, +size.z), normal2, color2)
          .addVertex(matrix.transformPoint(+size.x, +size.y, +size.z), normal2, color2)
          .addVertex(matrix.transformPoint(+size.x, +size.y, -size.z), normal2, color2);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, -size.z), normal3, color3)
          .addVertex(matrix.transformPoint(+size.x, -size.y, -size.z), normal3, color3)
          .addVertex(matrix.transformPoint(+size.x, -size.y, +size.z), normal3, color3)
          .addVertex(matrix.transformPoint(-size.x, -size.y, +size.z), normal3, color3);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(+size.x, -size.y, -size.z), normal4, color4)
          .addVertex(matrix.transformPoint(+size.x, +size.y, -size.z), normal4, color4)
          .addVertex(matrix.transformPoint(+size.x, +size.y, +size.z), normal4, color4)
          .addVertex(matrix.transformPoint(+size.x, -size.y, +size.z), normal4, color4);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(-size.x, -size.y, -size.z), normal5, color5)
          .addVertex(matrix.transformPoint(-size.x, -size.y, +size.z), normal5, color5)
          .addVertex(matrix.transformPoint(-size.x, +size.y, +size.z), normal5, color5)
          .addVertex(matrix.transformPoint(-size.x, +size.y, -size.z), normal5, color5);

        // ---

        batch
          .addIndices( 0,  2,  1,  0,  3,  2) /* FRONT */
          .addIndices( 4,  6,  5,  4,  7,  6) /* BACK */
          .addIndices( 8, 10,  9,  8, 11, 10) /* TOP */
          .addIndices(12, 14, 13, 12, 15, 14) /* BOTTOM */
          .addIndices(16, 18, 17, 16, 19, 18) /* RIGHT */
          .addIndices(20, 22, 21, 20, 23, 22) /* LEFT */
          .incrementIndices(24);
      }

      const glm::vec4& Cube::getFaceColor(int faceIndex) const
      {
        if (hasColors)
        {
          return colors[faceIndex];
        }
        else
        {
          return color;
        }
      }
    }
  }
}
