#pragma once

#include "path/ASPC.h"

namespace chr
{
  namespace path
  {
    template <typename T>
    class SplinePathBase : protected ASPC<T>
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

    template <typename T>
    class SplinePath : public SplinePathBase<T>
    {
    public:
      SplinePath(int capacity = 0)
      {
        if (capacity > 0)
        {
          SplinePathBase<T>::points.reserve(capacity);
        }
      }

      SplinePath(const std::vector<T> &points)
      {
        add(points);
      }

      const std::vector<T>& getPolyline()
      {
        SplinePathBase<T>::flush();
        return ASPC<T>::polyline;
      }

      const std::vector<T>& getPoints() const
      {
        return SplinePathBase<T>::points;
      }

      size_t size() const
      {
        return SplinePathBase<T>::points.size();
      }

      bool empty() const
      {
        return SplinePathBase<T>::points.empty();
      }

      bool isClosed() const
      {
        return SplinePathBase<T>::closed;
      }

      SplinePath& setSamplingTolerance(float tolerance)
      {
        ASPC<T>::samplingTolerance = tolerance;
        return *this;
      }

      SplinePath& setType(typename SplinePathBase<T>::Type type)
      {
        SplinePathBase<T>::type = type;
        return *this;
      }

      SplinePath& clear()
      {
        SplinePathBase<T>::flushed = false;

        SplinePathBase<T>::points.clear();
        ASPC<T>::polyline.clear();

        return *this;
      }

      SplinePath& close()
      {
        if (SplinePathBase<T>::points.size() > 2)
        {
          SplinePathBase<T>::closed = true;

          if (SplinePathBase<T>::points.front() == SplinePathBase<T>::points.back())
          {
            SplinePathBase<T>::points.pop_back();
          }
        }

        return *this;
      }

      SplinePath& add(const std::vector<T> &newPoints)
      {
        if (!SplinePathBase<T>::closed)
        {
          auto newSize = newPoints.size();

          if (newSize > 0)
          {
            SplinePathBase<T>::points.reserve(SplinePathBase<T>::points.size() + newSize);

            for (const auto &point : newPoints)
            {
              add(point);
            }

            SplinePathBase<T>::flushed = false;
          }
        }

        return *this;
      }

      SplinePath& add(const T &point)
      {
        if (!SplinePathBase<T>::closed && (SplinePathBase<T>::points.empty() || (point != SplinePathBase<T>::points.back())))
        {
          SplinePathBase<T>::points.emplace_back(point);
          SplinePathBase<T>::flushed = false;
        }

        return *this;
      }

      inline SplinePath& add(float x, float y) { return add(T(x, y)); }

      template <typename M>
      SplinePath& transformPoints(const M &matrix)
      {
        for (auto &point : SplinePathBase<T>::points)
        {
          point = matrix.transformPoint(point);
        }

        return *this;
      }
    };
  }
}
