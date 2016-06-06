#pragma once

#include "path/ASPC.h"
#include "math/MatrixAffine.h"

namespace chr
{
  namespace path
  {
    class SplinePath : public ASPC2D
    {
    public:
      enum Type
      {
        TYPE_BSPLINE,
        TYPE_CATMULL_ROM
      };

      SplinePath(int capacity = 0);
      SplinePath(const std::vector<glm::vec2> &points);

      const std::vector<glm::vec2>& getPolyline();
      const std::vector<glm::vec2>& getPoints() const;

      size_t size() const;
      bool empty() const;
      bool isClosed() const;

      SplinePath& setSamplingTolerance(float tolerance);
      SplinePath& setType(Type type);

      SplinePath& clear();
      SplinePath& close();

      SplinePath& add(const std::vector<glm::vec2> &points);
      SplinePath& add(const glm::vec2 &point);
      inline SplinePath& add(float x, float y) { return add(glm::vec2(x, y)); }

      SplinePath& transformPoints(const math::MatrixAffine &matrix);

    protected:
      std::vector<glm::vec2> points;

      Type type = TYPE_BSPLINE;
      bool closed = false;
      bool flushed = false;

      bool flush();

      static glm::vec2 GammaBSpline(float t, glm::vec2 *in)
      {
        return (in[0] * (((3 - t) * t - 3) * t + 1) + in[1] * (((3 * t - 6) * t) * t + 4) + in[2] * (((3 - 3 * t) * t + 3) * t + 1) + in[3] * (t * t * t)) / 6.0f;
      }

      static glm::vec2 GammaCatmullRom(float t, glm::vec2 *in)
      {
        return (in[0] * (((2 - t) * t - 1) * t) + in[1] * (((3 * t - 5) * t) * t + 2) + in[2] * (((4 - 3 * t) * t + 1) * t) + in[3] * ((t - 1) * t * t)) / 2.0f;
      }
    };
  }
}
