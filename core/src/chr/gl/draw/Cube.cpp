#include "chr/gl/draw/Cube.h"

using namespace chr;
using namespace math;

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      Cube& Cube::setSize(float size)
      {
        this->size = size;
        return *this;
      }

      Cube& Cube::setFrontFace(GLenum mode)
      {
        frontFace = mode;
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
      void Cube::append(IndexedVertexBatch<XYZ, GLushort> &batch, const Matrix &matrix) const
      {
        float unit = size * 0.5f;

        batch
          .addVertex(matrix.transformPoint(-unit, -unit, +unit))
          .addVertex(matrix.transformPoint(+unit, -unit, +unit))
          .addVertex(matrix.transformPoint(+unit, +unit, +unit))
          .addVertex(matrix.transformPoint(-unit, +unit, +unit))
          .addVertex(matrix.transformPoint(-unit, -unit, -unit))
          .addVertex(matrix.transformPoint(+unit, -unit, -unit))
          .addVertex(matrix.transformPoint(+unit, +unit, -unit))
          .addVertex(matrix.transformPoint(-unit, +unit, -unit));

        // ---

        if (frontFace == CW)
        {
          batch
            .addIndices(0, 1, 2, 2, 3, 0)
            .addIndices(1, 5, 6, 6, 2, 1)
            .addIndices(4, 7, 6, 6, 5, 4)
            .addIndices(0, 3, 7, 7, 4, 0)
            .addIndices(0, 4, 5, 5, 1, 0)
            .addIndices(3, 2, 6, 6, 7, 3);
        }
        else
        {
          batch
            .addIndices(0, 3, 2, 2, 1, 0)
            .addIndices(1, 2, 6, 6, 5, 1)
            .addIndices(4, 5, 6, 6, 7, 4)
            .addIndices(0, 4, 7, 7, 3, 0)
            .addIndices(0, 1, 5, 5, 4, 0)
            .addIndices(3, 7, 6, 6, 2, 3);
        }

        batch.incrementIndices(8);
      }

      template <>
      void Cube::append(IndexedVertexBatch<XYZ.N, GLushort> &batch, const Matrix &matrix) const
      {
        float unit = size * 0.5f;

        auto normal0 = matrix.transformNormal( 0,  0, +1);
        auto normal1 = matrix.transformNormal( 0,  0, -1);
        auto normal2 = matrix.transformNormal( 0, +1,  0);
        auto normal3 = matrix.transformNormal( 0, -1,  0);
        auto normal4 = matrix.transformNormal(+1,  0,  0);
        auto normal5 = matrix.transformNormal(-1,  0,  0);

        // FRONT
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), normal0)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), normal0)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), normal0)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), normal0);

        // BACK
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), normal1)
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), normal1)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), normal1)
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), normal1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), normal2)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), normal2)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), normal2)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), normal2);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), normal3)
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), normal3)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), normal3)
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), normal3);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), normal4)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), normal4)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), normal4)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), normal4);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), normal5)
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), normal5)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), normal5)
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), normal5);

        // ---

        if (frontFace == CW)
        {
          batch
            .addIndices( 0,  1,  2,  0,  2,  3)  /* FRONT */
            .addIndices( 4,  5,  6,  4,  6,  7)  /* BACK */
            .addIndices( 8,  9, 10,  8, 10, 11)  /* TOP */
            .addIndices(12, 13, 14, 12, 14, 15)  /* BOTTOM */
            .addIndices(16, 17, 18, 16, 18, 19)  /* RIGHT */
            .addIndices(20, 21, 22, 20, 22, 23); /* LEFT */
        }
        else
        {
          batch
            .addIndices( 0,  2,  1,  0,  3,  2)  /* FRONT */
            .addIndices( 4,  6,  5,  4,  7,  6)  /* BACK */
            .addIndices( 8, 10,  9,  8, 11, 10)  /* TOP */
            .addIndices(12, 14, 13, 12, 15, 14)  /* BOTTOM */
            .addIndices(16, 18, 17, 16, 19, 18)  /* RIGHT */
            .addIndices(20, 22, 21, 20, 23, 22); /* LEFT */
        }

        batch.incrementIndices(24);
      }

      template <>
      void Cube::append(IndexedVertexBatch<XYZ.UV, GLushort> &batch, const Matrix &matrix) const
      {
        float unit = size * 0.5f;

        // FRONT
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), 0, 0)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), 1, 0)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), 1, 1)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), 0, 1);

        // BACK
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), 0, 0)
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), 1, 0)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), 1, 1)
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), 0, 1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), 0, 0)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), 1, 0)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), 1, 1)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), 0, 1);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), 0, 0)
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), 1, 0)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), 1, 1)
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), 0, 1);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), 0, 0)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), 1, 0)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), 1, 1)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), 0, 1);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), 0, 0)
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), 1, 0)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), 1, 1)
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), 0, 1);

        // ---

        if (frontFace == CW)
        {
          batch
            .addIndices( 0,  1,  2,  0,  2,  3)  /* FRONT */
            .addIndices( 4,  5,  6,  4,  6,  7)  /* BACK */
            .addIndices( 8,  9, 10,  8, 10, 11)  /* TOP */
            .addIndices(12, 13, 14, 12, 14, 15)  /* BOTTOM */
            .addIndices(16, 17, 18, 16, 18, 19)  /* RIGHT */
            .addIndices(20, 21, 22, 20, 22, 23); /* LEFT */
        }
        else
        {
          batch
            .addIndices( 0,  2,  1,  0,  3,  2)  /* FRONT */
            .addIndices( 4,  6,  5,  4,  7,  6)  /* BACK */
            .addIndices( 8, 10,  9,  8, 11, 10)  /* TOP */
            .addIndices(12, 14, 13, 12, 15, 14)  /* BOTTOM */
            .addIndices(16, 18, 17, 16, 19, 18)  /* RIGHT */
            .addIndices(20, 22, 21, 20, 23, 22); /* LEFT */
        }

        batch.incrementIndices(24);
      }

      template <>
      void Cube::append(IndexedVertexBatch<XYZ.RGBA, GLushort> &batch, const Matrix &matrix) const
      {
        float unit = size * 0.5f;

        auto color0 = getFaceColor(0);
        auto color1 = getFaceColor(1);
        auto color2 = getFaceColor(2);
        auto color3 = getFaceColor(3);
        auto color4 = getFaceColor(4);
        auto color5 = getFaceColor(5);

        // FRONT
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), color0)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), color0)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), color0)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), color0);

        // BACK
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), color1)
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), color1)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), color1)
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), color1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), color2)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), color2)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), color2)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), color2);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), color3)
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), color3)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), color3)
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), color3);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), color4)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), color4)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), color4)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), color4);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), color5)
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), color5)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), color5)
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), color5);

        // ---

        if (frontFace == CW)
        {
          batch
            .addIndices( 0,  1,  2,  0,  2,  3)  /* FRONT */
            .addIndices( 4,  5,  6,  4,  6,  7)  /* BACK */
            .addIndices( 8,  9, 10,  8, 10, 11)  /* TOP */
            .addIndices(12, 13, 14, 12, 14, 15)  /* BOTTOM */
            .addIndices(16, 17, 18, 16, 18, 19)  /* RIGHT */
            .addIndices(20, 21, 22, 20, 22, 23); /* LEFT */
        }
        else
        {
          batch
            .addIndices( 0,  2,  1,  0,  3,  2)  /* FRONT */
            .addIndices( 4,  6,  5,  4,  7,  6)  /* BACK */
            .addIndices( 8, 10,  9,  8, 11, 10)  /* TOP */
            .addIndices(12, 14, 13, 12, 15, 14)  /* BOTTOM */
            .addIndices(16, 18, 17, 16, 19, 18)  /* RIGHT */
            .addIndices(20, 22, 21, 20, 23, 22); /* LEFT */
        }

        batch.incrementIndices(24);
      }

      template <>
      void Cube::append(IndexedVertexBatch<XYZ.N.UV, GLushort> &batch, const Matrix &matrix) const
      {
        float unit = size * 0.5f;

        auto normal0 = matrix.transformNormal( 0,  0, +1);
        auto normal1 = matrix.transformNormal( 0,  0, -1);
        auto normal2 = matrix.transformNormal( 0, +1,  0);
        auto normal3 = matrix.transformNormal( 0, -1,  0);
        auto normal4 = matrix.transformNormal(+1,  0,  0);
        auto normal5 = matrix.transformNormal(-1,  0,  0);

        // FRONT
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), normal0, 0, 0)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), normal0, 1, 0)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), normal0, 1, 1)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), normal0, 0, 1);

        // BACK
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), normal1, 0, 0)
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), normal1, 1, 0)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), normal1, 1, 1)
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), normal1, 0, 1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), normal2, 0, 0)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), normal2, 1, 0)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), normal2, 1, 1)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), normal2, 0, 1);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), normal3, 0, 0)
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), normal3, 1, 0)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), normal3, 1, 1)
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), normal3, 0, 1);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), normal4, 0, 0)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), normal4, 1, 0)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), normal4, 1, 1)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), normal4, 0, 1);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), normal5, 0, 0)
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), normal5, 1, 0)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), normal5, 1, 1)
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), normal5, 0, 1);

        // ---

        if (frontFace == CW)
        {
          batch
            .addIndices( 0,  1,  2,  0,  2,  3)  /* FRONT */
            .addIndices( 4,  5,  6,  4,  6,  7)  /* BACK */
            .addIndices( 8,  9, 10,  8, 10, 11)  /* TOP */
            .addIndices(12, 13, 14, 12, 14, 15)  /* BOTTOM */
            .addIndices(16, 17, 18, 16, 18, 19)  /* RIGHT */
            .addIndices(20, 21, 22, 20, 22, 23); /* LEFT */
        }
        else
        {
          batch
            .addIndices( 0,  2,  1,  0,  3,  2)  /* FRONT */
            .addIndices( 4,  6,  5,  4,  7,  6)  /* BACK */
            .addIndices( 8, 10,  9,  8, 11, 10)  /* TOP */
            .addIndices(12, 14, 13, 12, 15, 14)  /* BOTTOM */
            .addIndices(16, 18, 17, 16, 19, 18)  /* RIGHT */
            .addIndices(20, 22, 21, 20, 23, 22); /* LEFT */
        }

        batch.incrementIndices(24);
      }

      template <>
      void Cube::append(IndexedVertexBatch<XYZ.N.RGBA, GLushort> &batch, const Matrix &matrix) const
      {
        float unit = size * 0.5f;
        
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
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), normal0, color0)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), normal0, color0)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), normal0, color0)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), normal0, color0);

        // BACK
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), normal1, color1)
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), normal1, color1)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), normal1, color1)
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), normal1, color1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), normal2, color2)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), normal2, color2)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), normal2, color2)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), normal2, color2);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), normal3, color3)
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), normal3, color3)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), normal3, color3)
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), normal3, color3);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(+unit, -unit, -unit), normal4, color4)
          .addVertex(matrix.transformPoint(+unit, +unit, -unit), normal4, color4)
          .addVertex(matrix.transformPoint(+unit, +unit, +unit), normal4, color4)
          .addVertex(matrix.transformPoint(+unit, -unit, +unit), normal4, color4);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(-unit, -unit, -unit), normal5, color5)
          .addVertex(matrix.transformPoint(-unit, -unit, +unit), normal5, color5)
          .addVertex(matrix.transformPoint(-unit, +unit, +unit), normal5, color5)
          .addVertex(matrix.transformPoint(-unit, +unit, -unit), normal5, color5);

        // ---

        if (frontFace == CW)
        {
          batch
            .addIndices( 0,  1,  2,  0,  2,  3)  /* FRONT */
            .addIndices( 4,  5,  6,  4,  6,  7)  /* BACK */
            .addIndices( 8,  9, 10,  8, 10, 11)  /* TOP */
            .addIndices(12, 13, 14, 12, 14, 15)  /* BOTTOM */
            .addIndices(16, 17, 18, 16, 18, 19)  /* RIGHT */
            .addIndices(20, 21, 22, 20, 22, 23); /* LEFT */
        }
        else
        {
          batch
            .addIndices( 0,  2,  1,  0,  3,  2)  /* FRONT */
            .addIndices( 4,  6,  5,  4,  7,  6)  /* BACK */
            .addIndices( 8, 10,  9,  8, 11, 10)  /* TOP */
            .addIndices(12, 14, 13, 12, 15, 14)  /* BOTTOM */
            .addIndices(16, 18, 17, 16, 19, 18)  /* RIGHT */
            .addIndices(20, 22, 21, 20, 23, 22); /* LEFT */
        }

        batch.incrementIndices(24);
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
