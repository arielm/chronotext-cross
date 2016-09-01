#include "chr/gl/Revolver.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    Revolver& Revolver::setFrontFace(GLenum mode)
    {
      frontFace = mode;
      return *this;
    }

    Revolver& Revolver::setTextureOffset(const glm::vec2 &offset)
    {
      textureOffset = offset;
      return *this;
    }

    Revolver& Revolver::setTextureOffset(float x, float y)
    {
      textureOffset = glm::vec2(x, y);
      return *this;
    }

    Revolver& Revolver::setTextureScale(float scale)
    {
      textureScale = { scale, scale };
      return *this;
    }

    Revolver& Revolver::setTextureScale(float scaleX, float scaleY)
    {
      textureScale = { scaleX, scaleY };
      return *this;
    }

    Revolver& Revolver::setColor(const glm::vec4 &color)
    {
      this->color = color;
      return *this;
    }

    Revolver& Revolver::setColor(float r, float g, float b, float a)
    {
      color = { r, g, b, a };
      return *this;
    }

    Revolver& Revolver::setCapFront(bool cap)
    {
      capFront = cap;
      return  *this;
    }

    Revolver& Revolver::setCapBack(bool cap)
    {
      capBack = cap;
      return  *this;
    }

    Revolver& Revolver::setArc(float a1, float a2)
    {
      this->a1 = a1;
      this->a2 = a2;

      return *this;
    }

    Revolver& Revolver::setSegmentLength(float length)
    {
      segmentLength = length;
      return *this;
    }

    Revolver& Revolver::setContourCapture(int capture)
    {
      contourCapture = capture;
      return *this;
    }

    // ---

    template <>
    bool Revolver::process(IndexedVertexBatch<XYZ> &batch, const Matrix &matrix, const vector<glm::vec2> &polyline)
    {
      if (scanProfile(polyline))
      {
        performProcess(batch, matrix, polyline);
        return true;
      }

      return false;
    }

    template <>
    bool Revolver::process(IndexedVertexBatch<XYZ.N> &batch, const Matrix &matrix, const vector<glm::vec2> &polyline)
    {
      if (scanProfile(polyline))
      {
        performProcessWithNormals(batch, matrix, polyline);
        return true;
      }

      return false;
    }

    template <>
    bool Revolver::process(IndexedVertexBatch<XYZ.UV> &batch, const Matrix &matrix, const vector<glm::vec2> &polyline)
    {
      if (scanProfile(polyline))
      {
        performProcessWithTexture(batch, matrix, polyline);
        return true;
      }

      return false;
    }

    template <>
    bool Revolver::process(IndexedVertexBatch<XYZ.RGBA> &batch, const Matrix &matrix, const vector<glm::vec2> &polyline)
    {
      if (scanProfile(polyline))
      {
        performProcess(batch, matrix, polyline, color);
        return true;
      }

      return false;
    }

    template <>
    bool Revolver::process(IndexedVertexBatch<XYZ.N.RGBA> &batch, const Matrix &matrix, const vector<glm::vec2> &polyline)
    {
      if (scanProfile(polyline))
      {
        performProcessWithNormals(batch, matrix, polyline, color);
        return true;
      }

      return false;
    }

    template <>
    bool Revolver::process(IndexedVertexBatch<XYZ.UV.RGBA> &batch, const Matrix &matrix, const vector<glm::vec2> &polyline)
    {
      if (scanProfile(polyline))
      {
        performProcessWithTexture(batch, matrix, polyline, color);
        return true;
      }

      return false;
    }

    template <>
    bool Revolver::process(IndexedVertexBatch<XYZ.N.UV.RGBA> &batch, const Matrix &matrix, const vector<glm::vec2> &polyline)
    {
      if (scanProfile(polyline))
      {
        performProcessWithNormalsAndTexture(batch, matrix, polyline, color);
        return true;
      }

      return false;
    }

    // ---

    bool Revolver::exportContours(IndexedVertexBatch<XYZ> &batch) const
    {
      if (!contourVertices.empty())
      {
        batch.addVertices(contourVertices); // XXX

        bool frontCapExported = false;
        bool backCapExported = false;

        if ((contourCapture == CAPTURE_CAP_FRONT) || (contourCapture == CAPTURE_CAP_BACK))
        {
          for (auto column = 0; column < contourColumnCount - 1; column++)
          {
            auto i0 = column;
            auto i1 = i0 + 1;

            batch.addIndices(i0, i1);
          }

          frontCapExported = (contourCapture == CAPTURE_CAP_FRONT);
          backCapExported = (contourCapture == CAPTURE_CAP_BACK);
        }
        else if (contourCapture == CAPTURE_CAPS)
        {
          for (auto column = 0; column < contourColumnCount - 1; column++)
          {
            for (auto row = 0; row < 2; row++)
            {
              auto i0 = column * 2 + row;
              auto i1 = i0 + 2;

              batch.addIndices(i0, i1);
            }
          }

          frontCapExported = true;
          backCapExported = true;
        }

        if (contourCapture & CAPTURE_ROWS)
        {
          for (auto column = 0; column < contourColumnCount - 1; column++)
          {
            for (auto row = 0; row < contourRowCount; row++)
            {
              auto i0 = column * contourRowCount + row;
              auto i1 = i0 + contourRowCount;

              if (row == 0)
              {
                if (!frontCapExported)
                {
                  batch.addIndices(i0, i1);
                }
              }
              else if (row == contourRowCount - 1)
              {
                if (!backCapExported)
                {
                  batch.addIndices(i0, i1);
                }
              }
              else
              {
                batch.addIndices(i0, i1);
              }
            }
          }
        }

        /*
         * FIXME: THE FOLLOWING IS BAD IN TERM OF VERTEX-LOCALITY
         */

        if (contourCapture & CAPTURE_COLUMNS)
        {
          for (auto column = 0; column < contourColumnCount -1; column++)
          {
            for (auto row = 0; row < contourRowCount; row++)
            {
              auto i0 = column * contourRowCount + row;
              auto i1 = i0 + contourRowCount;
              auto i2 = i0 + 1;

              if (row == contourRowCount - 1)
              {
                if (!backCapExported && (contourCapture & CAPTURE_CAP_BACK))
                {
                  batch.addIndices(i0, i1);
                }
              }
              else
              {
                if (row == 0)
                {
                  if (!frontCapExported && (contourCapture & CAPTURE_CAP_FRONT))
                  {
                    batch.addIndices(i0, i1);
                  }
                }

                batch.addIndices(i0, i2);
              }
            }
          }
        }

        batch.incrementIndices(contourVertices.size());

        return bool(contourCapture);
      }

      return false;
    }

    // ---

    bool Revolver::scanProfile(const vector<glm::vec2> &polyline)
    {
      minRadius = FLT_MAX;
      maxRadius = 0;

      if (polyline.size() > 1)
      {
        float y = polyline.front().y;
        bool topDown = (y < polyline.back().y);

        for (const auto &point : polyline)
        {
          if ((topDown && (point.y < y)) || (!topDown && (point.y > y)))
          {
            return false;
          }

          y = point.y;

          minRadius = fminf(minRadius, point.x);
          maxRadius = fmaxf(maxRadius, point.x);
        }

        return true;
      }

      return false;
    }

    glm::vec2 Revolver::getTextureCoords(const Texture &texture, const glm::vec2 &xy) const
    {
      return (xy - textureOffset) / (texture.innerSize * textureScale);
    }
  }
}
