#pragma once

#include "path/ASPC.h"
#include "math/MatrixAffine.h"

namespace chr
{
  namespace path
  {
    template <typename T>
    class Spline
    {
    public:
      enum Type
      {
        TYPE_BSPLINE,
        TYPE_CATMULL_ROM
      };

      static T GammaBSpline(float t, T *in)
      {
        return (
          in[0] * (((3 - t) * t - 3) * t + 1) +
          in[1] * (((3 * t - 6) * t) * t + 4) +
          in[2] * (((3 - 3 * t) * t + 3) * t + 1) +
          in[3] * (t * t * t)
        ) / 6.0f;
      }

      static T GammaCatmullRom(float t, T *in)
      {
        return (
          in[0] * (((2 - t) * t - 1) * t) +
          in[1] * (((3 * t - 5) * t) * t + 2) +
          in[2] * (((4 - 3 * t) * t + 1) * t) +
          in[3] * ((t - 1) * t * t)
        ) / 2.0f;
      }

    protected:
      std::vector<T> points;

      Type type = TYPE_BSPLINE;
      bool closed = false;
      bool flushed = false;
    };

    class SplinePath2D : public Spline<glm::vec2>, ASPC<glm::vec2>
    {
    public:
      SplinePath2D(int capacity = 0);
      SplinePath2D(const std::vector<glm::vec2> &points);

      const std::vector<glm::vec2>& getPolyline();
      const std::vector<glm::vec2>& getPoints() const;

      size_t size() const;
      bool empty() const;
      bool isClosed() const;

      SplinePath2D& setSamplingTolerance(float tolerance);
      SplinePath2D& setType(Type type);

      SplinePath2D& clear();
      SplinePath2D& close();

      SplinePath2D& add(const std::vector<glm::vec2> &points);
      SplinePath2D& add(const glm::vec2 &point);
      inline SplinePath2D& add(float x, float y) { return add(glm::vec2(x, y)); }

      SplinePath2D& transformPoints(const math::MatrixAffine &matrix);

    protected:
      bool flush();
    };
  }
}
