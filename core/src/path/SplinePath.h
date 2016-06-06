#pragma once

#include "path/ASPC.h"
#include "math/MatrixAffine.h"

namespace chr
{
  namespace path
  {
    template <typename T>
    class Spline : protected ASPC<T>
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

      bool flush()
      {
        if (!flushed)
        {
          auto size = points.size();

          if (size > 2)
          {
            ASPC<T>::begin();

            switch (type)
            {
              case TYPE_BSPLINE:
                ASPC<T>::gamma = GammaBSpline;
                break;

              case TYPE_CATMULL_ROM:
                ASPC<T>::gamma = GammaCatmullRom;
                break;
            }

            if (closed)
            {
              ASPC<T>::segment(points[size - 1], points[0], points[1], points[2]);
            }
            else
            {
              if (type == TYPE_BSPLINE)
              {
                ASPC<T>::segment(points[0], points[0], points[0], points[1]);
              }

              ASPC<T>::segment(points[0], points[0], points[1], points[2]);
            }

            for (auto i = 0; i < size - 3; i++)
            {
              ASPC<T>::segment(points[i], points[i + 1], points[i + 2], points[i + 3]);
            }

            if (closed)
            {
              ASPC<T>::segment(points[size - 3], points[size - 2], points[size - 1], points[0]);
              ASPC<T>::segment(points[size - 2], points[size - 1], points[0], points[1]);

              if (ASPC<T>::polyline.front() != ASPC<T>::polyline.back())
              {
                ASPC<T>::polyline.emplace_back(ASPC<T>::polyline.front());
              }
            }
            else
            {
              ASPC<T>::segment(points[size - 3], points[size - 2], points[size - 1], points[size - 1]);
              ASPC<T>::segment(points[size - 2], points[size - 1], points[size - 1], points[size - 1]);

              if (type == TYPE_BSPLINE)
              {
                ASPC<T>::segment(points[size - 1], points[size - 1], points[size - 1], points[size - 1]);
              }
            }

            flushed = true;
          }

          return flushed;
        }

        return false;
      }
    };

    class SplinePath2D : public Spline<glm::vec2>
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
    };
  }
}
