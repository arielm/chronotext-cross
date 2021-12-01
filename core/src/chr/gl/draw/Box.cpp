#include "chr/gl/draw/Box.h"

using namespace chr;
using namespace math;

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      Box& Box::setSize(float width, float height, float depth)
      {
        this->width = width;
        this->height = height;
        this->depth = depth;

        return *this;
      }

      Box& Box::setFrontFace(GLenum mode)
      {
        frontFace = mode;
        return *this;
      }

      Box& Box::setTextureOffset(const glm::vec2 &offset)
      {
        textureOffset = offset;
        return *this;
      }

      Box& Box::setTextureOffset(float x, float y)
      {
        textureOffset = glm::vec2(x, y);
        return *this;
      }

      Box& Box::setTextureScale(float scale)
      {
        textureScale = { scale, scale };
        return *this;
      }

      Box& Box::setTextureScale(float scaleX, float scaleY)
      {
        textureScale = { scaleX, scaleY };
        return *this;
      }

      Box& Box::setColor(const glm::vec4 &color)
      {
        this->color = color;
        return *this;
      }

      Box& Box::setColor(float r, float g, float b, float a)
      {
        color = { r, g, b, a };
        return *this;
      }

      template <>
      void Box::append(IndexedVertexBatch<XYZ, GLuint> &batch, const Matrix &matrix) const
      {
        float x0 = 0, x1 = width;
        float y0 = 0, y1 = height;
        float z0 = 0, z1 = depth;

        if (x0 > x1) std::swap(x0, x1);
        if (y0 > y1) std::swap(y0, y1);
        if (z0 > z1) std::swap(z0, z1);

        batch
          .addVertex(matrix.transformPoint(x0, y0, z1))
          .addVertex(matrix.transformPoint(x1, y0, z1))
          .addVertex(matrix.transformPoint(x1, y1, z1))
          .addVertex(matrix.transformPoint(x0, y1, z1))
          .addVertex(matrix.transformPoint(x0, y0, z0))
          .addVertex(matrix.transformPoint(x1, y0, z0))
          .addVertex(matrix.transformPoint(x1, y1, z0))
          .addVertex(matrix.transformPoint(x0, y1, z0));

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
      void Box::append(IndexedVertexBatch<XYZ.N, GLuint> &batch, const Matrix &matrix) const
      {
        float x0 = 0, x1 = width;
        float y0 = 0, y1 = height;
        float z0 = 0, z1 = depth;

        if (x0 > x1) std::swap(x0, x1);
        if (y0 > y1) std::swap(y0, y1);
        if (z0 > z1) std::swap(z0, z1);

        auto normal0 = matrix.transformNormal( 0,  0, +1);
        auto normal1 = matrix.transformNormal( 0,  0, -1);
        auto normal2 = matrix.transformNormal( 0, +1,  0);
        auto normal3 = matrix.transformNormal( 0, -1,  0);
        auto normal4 = matrix.transformNormal(+1,  0,  0);
        auto normal5 = matrix.transformNormal(-1,  0,  0);

        // FRONT
        batch
          .addVertex(matrix.transformPoint(x0, y0, z1), normal0)
          .addVertex(matrix.transformPoint(x1, y0, z1), normal0)
          .addVertex(matrix.transformPoint(x1, y1, z1), normal0)
          .addVertex(matrix.transformPoint(x0, y1, z1), normal0);

        // BACK
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), normal1)
          .addVertex(matrix.transformPoint(x0, y1, z0), normal1)
          .addVertex(matrix.transformPoint(x1, y1, z0), normal1)
          .addVertex(matrix.transformPoint(x1, y0, z0), normal1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(x0, y1, z0), normal2)
          .addVertex(matrix.transformPoint(x0, y1, z1), normal2)
          .addVertex(matrix.transformPoint(x1, y1, z1), normal2)
          .addVertex(matrix.transformPoint(x1, y1, z0), normal2);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), normal3)
          .addVertex(matrix.transformPoint(x1, y0, z0), normal3)
          .addVertex(matrix.transformPoint(x1, y0, z1), normal3)
          .addVertex(matrix.transformPoint(x0, y0, z1), normal3);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(x1, y0, z0), normal4)
          .addVertex(matrix.transformPoint(x1, y1, z0), normal4)
          .addVertex(matrix.transformPoint(x1, y1, z1), normal4)
          .addVertex(matrix.transformPoint(x1, y0, z1), normal4);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), normal5)
          .addVertex(matrix.transformPoint(x0, y0, z1), normal5)
          .addVertex(matrix.transformPoint(x0, y1, z1), normal5)
          .addVertex(matrix.transformPoint(x0, y1, z0), normal5);

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
      void Box::append(IndexedVertexBatch<XYZ.UV, GLuint> &batch, const Matrix &matrix) const
      {
        float x0 = 0, x1 = width;
        float y0 = 0, y1 = height;
        float z0 = 0, z1 = depth;

        if (x0 > x1) std::swap(x0, x1);
        if (y0 > y1) std::swap(y0, y1);
        if (z0 > z1) std::swap(z0, z1);

        glm::vec2 coord0;
        glm::vec2 coord1;
        glm::vec2 coord2;
        glm::vec2 coord3;
        glm::vec2 coord4;
        glm::vec2 coord5;
        glm::vec2 coord6;
        glm::vec2 coord7;

        if (batch->hasTexture)
        {
          coord0 = getTextureCoords(batch->texture, 0, 0);
          coord1 = getTextureCoords(batch->texture, width, 0);
          coord2 = getTextureCoords(batch->texture, width, height);
          coord3 = getTextureCoords(batch->texture, 0, height);
          coord4 = getTextureCoords(batch->texture, 0, depth);
          coord5 = getTextureCoords(batch->texture, width, depth);
          coord6 = getTextureCoords(batch->texture, height, 0);
          coord7 = getTextureCoords(batch->texture, height, depth);
        }
        else
        {
          coord0 = {0, 0};
          coord1 = {1, 0};
          coord2 = {1, 1};
          coord3 = {0, 1};
          coord4 = {0, 1};
          coord5 = {1, 1};
          coord6 = {1, 0};
          coord7 = {1, 1};
        }

        // FRONT
        batch
          .addVertex(matrix.transformPoint(x0, y0, z1), coord0)
          .addVertex(matrix.transformPoint(x1, y0, z1), coord1)
          .addVertex(matrix.transformPoint(x1, y1, z1), coord2)
          .addVertex(matrix.transformPoint(x0, y1, z1), coord3);

        // BACK
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), coord0)
          .addVertex(matrix.transformPoint(x0, y1, z0), coord3)
          .addVertex(matrix.transformPoint(x1, y1, z0), coord2)
          .addVertex(matrix.transformPoint(x1, y0, z0), coord1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(x0, y1, z0), coord0)
          .addVertex(matrix.transformPoint(x0, y1, z1), coord4)
          .addVertex(matrix.transformPoint(x1, y1, z1), coord5)
          .addVertex(matrix.transformPoint(x1, y1, z0), coord1);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), coord0)
          .addVertex(matrix.transformPoint(x1, y0, z0), coord1)
          .addVertex(matrix.transformPoint(x1, y0, z1), coord5)
          .addVertex(matrix.transformPoint(x0, y0, z1), coord4);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(x1, y0, z0), coord0)
          .addVertex(matrix.transformPoint(x1, y1, z0), coord6)
          .addVertex(matrix.transformPoint(x1, y1, z1), coord7)
          .addVertex(matrix.transformPoint(x1, y0, z1), coord4);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), coord0)
          .addVertex(matrix.transformPoint(x0, y0, z1), coord4)
          .addVertex(matrix.transformPoint(x0, y1, z1), coord7)
          .addVertex(matrix.transformPoint(x0, y1, z0), coord6);

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
      void Box::append(IndexedVertexBatch<XYZ.RGBA, GLuint> &batch, const Matrix &matrix) const
      {
        float x0 = 0, x1 = width;
        float y0 = 0, y1 = height;
        float z0 = 0, z1 = depth;

        if (x0 > x1) std::swap(x0, x1);
        if (y0 > y1) std::swap(y0, y1);
        if (z0 > z1) std::swap(z0, z1);

        auto color0 = getFaceColor(0);
        auto color1 = getFaceColor(1);
        auto color2 = getFaceColor(2);
        auto color3 = getFaceColor(3);
        auto color4 = getFaceColor(4);
        auto color5 = getFaceColor(5);

        // FRONT
        batch
          .addVertex(matrix.transformPoint(x0, y0, z1), color0)
          .addVertex(matrix.transformPoint(x1, y0, z1), color0)
          .addVertex(matrix.transformPoint(x1, y1, z1), color0)
          .addVertex(matrix.transformPoint(x0, y1, z1), color0);

        // BACK
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), color1)
          .addVertex(matrix.transformPoint(x0, y1, z0), color1)
          .addVertex(matrix.transformPoint(x1, y1, z0), color1)
          .addVertex(matrix.transformPoint(x1, y0, z0), color1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(x0, y1, z0), color2)
          .addVertex(matrix.transformPoint(x0, y1, z1), color2)
          .addVertex(matrix.transformPoint(x1, y1, z1), color2)
          .addVertex(matrix.transformPoint(x1, y1, z0), color2);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), color3)
          .addVertex(matrix.transformPoint(x1, y0, z0), color3)
          .addVertex(matrix.transformPoint(x1, y0, z1), color3)
          .addVertex(matrix.transformPoint(x0, y0, z1), color3);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(x1, y0, z0), color4)
          .addVertex(matrix.transformPoint(x1, y1, z0), color4)
          .addVertex(matrix.transformPoint(x1, y1, z1), color4)
          .addVertex(matrix.transformPoint(x1, y0, z1), color4);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), color5)
          .addVertex(matrix.transformPoint(x0, y0, z1), color5)
          .addVertex(matrix.transformPoint(x0, y1, z1), color5)
          .addVertex(matrix.transformPoint(x0, y1, z0), color5);

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
      void Box::append(IndexedVertexBatch<XYZ.N.UV, GLuint> &batch, const Matrix &matrix) const
      {
        float x0 = 0, x1 = width;
        float y0 = 0, y1 = height;
        float z0 = 0, z1 = depth;

        if (x0 > x1) std::swap(x0, x1);
        if (y0 > y1) std::swap(y0, y1);
        if (z0 > z1) std::swap(z0, z1);

        auto normal0 = matrix.transformNormal( 0,  0, +1);
        auto normal1 = matrix.transformNormal( 0,  0, -1);
        auto normal2 = matrix.transformNormal( 0, +1,  0);
        auto normal3 = matrix.transformNormal( 0, -1,  0);
        auto normal4 = matrix.transformNormal(+1,  0,  0);
        auto normal5 = matrix.transformNormal(-1,  0,  0);

        glm::vec2 coord0;
        glm::vec2 coord1;
        glm::vec2 coord2;
        glm::vec2 coord3;
        glm::vec2 coord4;
        glm::vec2 coord5;
        glm::vec2 coord6;
        glm::vec2 coord7;

        if (batch->hasTexture)
        {
          coord0 = getTextureCoords(batch->texture, 0, 0);
          coord1 = getTextureCoords(batch->texture, width, 0);
          coord2 = getTextureCoords(batch->texture, width, height);
          coord3 = getTextureCoords(batch->texture, 0, height);
          coord4 = getTextureCoords(batch->texture, 0, depth);
          coord5 = getTextureCoords(batch->texture, width, depth);
          coord6 = getTextureCoords(batch->texture, height, 0);
          coord7 = getTextureCoords(batch->texture, height, depth);
        }
        else
        {
          coord0 = {0, 0};
          coord1 = {1, 0};
          coord2 = {1, 1};
          coord3 = {0, 1};
          coord4 = {0, 1};
          coord5 = {1, 1};
          coord6 = {1, 0};
          coord7 = {1, 1};
        }

        // FRONT
        batch
          .addVertex(matrix.transformPoint(x0, y0, z1), normal0, coord0)
          .addVertex(matrix.transformPoint(x1, y0, z1), normal0, coord1)
          .addVertex(matrix.transformPoint(x1, y1, z1), normal0, coord2)
          .addVertex(matrix.transformPoint(x0, y1, z1), normal0, coord3);

        // BACK
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), normal1, coord0)
          .addVertex(matrix.transformPoint(x0, y1, z0), normal1, coord3)
          .addVertex(matrix.transformPoint(x1, y1, z0), normal1, coord2)
          .addVertex(matrix.transformPoint(x1, y0, z0), normal1, coord1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(x0, y1, z0), normal2, coord0)
          .addVertex(matrix.transformPoint(x0, y1, z1), normal2, coord4)
          .addVertex(matrix.transformPoint(x1, y1, z1), normal2, coord5)
          .addVertex(matrix.transformPoint(x1, y1, z0), normal2, coord1);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), normal3, coord0)
          .addVertex(matrix.transformPoint(x1, y0, z0), normal3, coord1)
          .addVertex(matrix.transformPoint(x1, y0, z1), normal3, coord5)
          .addVertex(matrix.transformPoint(x0, y0, z1), normal3, coord4);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(x1, y0, z0), normal4, coord0)
          .addVertex(matrix.transformPoint(x1, y1, z0), normal4, coord6)
          .addVertex(matrix.transformPoint(x1, y1, z1), normal4, coord7)
          .addVertex(matrix.transformPoint(x1, y0, z1), normal4, coord4);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), normal5, coord0)
          .addVertex(matrix.transformPoint(x0, y0, z1), normal5, coord4)
          .addVertex(matrix.transformPoint(x0, y1, z1), normal5, coord7)
          .addVertex(matrix.transformPoint(x0, y1, z0), normal5, coord6);

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
      void Box::append(IndexedVertexBatch<XYZ.N.RGBA, GLuint> &batch, const Matrix &matrix) const
      {
        float x0 = 0, x1 = width;
        float y0 = 0, y1 = height;
        float z0 = 0, z1 = depth;

        if (x0 > x1) std::swap(x0, x1);
        if (y0 > y1) std::swap(y0, y1);
        if (z0 > z1) std::swap(z0, z1);

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
          .addVertex(matrix.transformPoint(x0, y0, z1), normal0, color0)
          .addVertex(matrix.transformPoint(x1, y0, z1), normal0, color0)
          .addVertex(matrix.transformPoint(x1, y1, z1), normal0, color0)
          .addVertex(matrix.transformPoint(x0, y1, z1), normal0, color0);

        // BACK
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), normal1, color1)
          .addVertex(matrix.transformPoint(x0, y1, z0), normal1, color1)
          .addVertex(matrix.transformPoint(x1, y1, z0), normal1, color1)
          .addVertex(matrix.transformPoint(x1, y0, z0), normal1, color1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(x0, y1, z0), normal2, color2)
          .addVertex(matrix.transformPoint(x0, y1, z1), normal2, color2)
          .addVertex(matrix.transformPoint(x1, y1, z1), normal2, color2)
          .addVertex(matrix.transformPoint(x1, y1, z0), normal2, color2);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), normal3, color3)
          .addVertex(matrix.transformPoint(x1, y0, z0), normal3, color3)
          .addVertex(matrix.transformPoint(x1, y0, z1), normal3, color3)
          .addVertex(matrix.transformPoint(x0, y0, z1), normal3, color3);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(x1, y0, z0), normal4, color4)
          .addVertex(matrix.transformPoint(x1, y1, z0), normal4, color4)
          .addVertex(matrix.transformPoint(x1, y1, z1), normal4, color4)
          .addVertex(matrix.transformPoint(x1, y0, z1), normal4, color4);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), normal5, color5)
          .addVertex(matrix.transformPoint(x0, y0, z1), normal5, color5)
          .addVertex(matrix.transformPoint(x0, y1, z1), normal5, color5)
          .addVertex(matrix.transformPoint(x0, y1, z0), normal5, color5);

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
      void Box::append(IndexedVertexBatch<XYZ.N.UV.RGBA, GLuint> &batch, const Matrix &matrix) const
      {
        float x0 = 0, x1 = width;
        float y0 = 0, y1 = height;
        float z0 = 0, z1 = depth;

        if (x0 > x1) std::swap(x0, x1);
        if (y0 > y1) std::swap(y0, y1);
        if (z0 > z1) std::swap(z0, z1);

        auto normal0 = matrix.transformNormal( 0,  0, +1);
        auto normal1 = matrix.transformNormal( 0,  0, -1);
        auto normal2 = matrix.transformNormal( 0, +1,  0);
        auto normal3 = matrix.transformNormal( 0, -1,  0);
        auto normal4 = matrix.transformNormal(+1,  0,  0);
        auto normal5 = matrix.transformNormal(-1,  0,  0);

        glm::vec2 coord0;
        glm::vec2 coord1;
        glm::vec2 coord2;
        glm::vec2 coord3;
        glm::vec2 coord4;
        glm::vec2 coord5;
        glm::vec2 coord6;
        glm::vec2 coord7;

        if (batch->hasTexture)
        {
          coord0 = getTextureCoords(batch->texture, 0, 0);
          coord1 = getTextureCoords(batch->texture, width, 0);
          coord2 = getTextureCoords(batch->texture, width, height);
          coord3 = getTextureCoords(batch->texture, 0, height);
          coord4 = getTextureCoords(batch->texture, 0, depth);
          coord5 = getTextureCoords(batch->texture, width, depth);
          coord6 = getTextureCoords(batch->texture, height, 0);
          coord7 = getTextureCoords(batch->texture, height, depth);
        }
        else
        {
          coord0 = {0, 0};
          coord1 = {1, 0};
          coord2 = {1, 1};
          coord3 = {0, 1};
          coord4 = {0, 1};
          coord5 = {1, 1};
          coord6 = {1, 0};
          coord7 = {1, 1};
        }

        auto color0 = getFaceColor(0);
        auto color1 = getFaceColor(1);
        auto color2 = getFaceColor(2);
        auto color3 = getFaceColor(3);
        auto color4 = getFaceColor(4);
        auto color5 = getFaceColor(5);

        // FRONT
        batch
          .addVertex(matrix.transformPoint(x0, y0, z1), normal0, coord0, color0)
          .addVertex(matrix.transformPoint(x1, y0, z1), normal0, coord1, color0)
          .addVertex(matrix.transformPoint(x1, y1, z1), normal0, coord2, color0)
          .addVertex(matrix.transformPoint(x0, y1, z1), normal0, coord3, color0);

        // BACK
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), normal1, coord0, color1)
          .addVertex(matrix.transformPoint(x0, y1, z0), normal1, coord3, color1)
          .addVertex(matrix.transformPoint(x1, y1, z0), normal1, coord2, color1)
          .addVertex(matrix.transformPoint(x1, y0, z0), normal1, coord1, color1);

        // TOP
        batch
          .addVertex(matrix.transformPoint(x0, y1, z0), normal2, coord0, color2)
          .addVertex(matrix.transformPoint(x0, y1, z1), normal2, coord4, color2)
          .addVertex(matrix.transformPoint(x1, y1, z1), normal2, coord5, color2)
          .addVertex(matrix.transformPoint(x1, y1, z0), normal2, coord1, color2);

        // BOTTOM
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), normal3, coord0, color3)
          .addVertex(matrix.transformPoint(x1, y0, z0), normal3, coord1, color3)
          .addVertex(matrix.transformPoint(x1, y0, z1), normal3, coord5, color3)
          .addVertex(matrix.transformPoint(x0, y0, z1), normal3, coord4, color3);

        // RIGHT
        batch
          .addVertex(matrix.transformPoint(x1, y0, z0), normal4, coord0, color4)
          .addVertex(matrix.transformPoint(x1, y1, z0), normal4, coord6, color4)
          .addVertex(matrix.transformPoint(x1, y1, z1), normal4, coord7, color4)
          .addVertex(matrix.transformPoint(x1, y0, z1), normal4, coord4, color4);

        // LEFT
        batch
          .addVertex(matrix.transformPoint(x0, y0, z0), normal5, coord0, color5)
          .addVertex(matrix.transformPoint(x0, y0, z1), normal5, coord4, color5)
          .addVertex(matrix.transformPoint(x0, y1, z1), normal5, coord7, color5)
          .addVertex(matrix.transformPoint(x0, y1, z0), normal5, coord6, color5);

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

      // ---

      const glm::vec4& Box::getFaceColor(int faceIndex) const
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

      glm::vec2 Box::getTextureCoords(const Texture &texture, float x, float y) const
      {
        return (glm::vec2(x, y) - textureOffset) / (texture.innerSize * textureScale);
      }
    }
  }
}
