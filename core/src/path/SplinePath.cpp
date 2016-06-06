#include "path/SplinePath.h"

using namespace std;
using namespace chr;
using namespace math;

namespace chr
{
  namespace path
  {
    SplinePath2D::SplinePath2D(int capacity)
    {
      if (capacity > 0)
      {
        points.reserve(capacity);
      }
    }

    SplinePath2D::SplinePath2D(const std::vector <glm::vec2> &points)
    {
      add(points);
    }

    const vector<glm::vec2>& SplinePath2D::getPolyline()
    {
      flush();
      return polyline;
    }

    const vector<glm::vec2>& SplinePath2D::getPoints() const
    {
      return points;
    }

    size_t SplinePath2D::size() const
    {
      return points.size();
    }

    bool SplinePath2D::empty() const
    {
      return points.empty();
    }

    bool SplinePath2D::isClosed() const
    {
      return closed;
    }

    SplinePath2D &SplinePath2D::setSamplingTolerance(float tolerance)
    {
      samplingTolerance = tolerance;
      return *this;
    }

    SplinePath2D &SplinePath2D::setType(Type type)
    {
      this->type = type;
      return *this;
    }

    SplinePath2D &SplinePath2D::clear()
    {
      flushed = false;

      points.clear();
      polyline.clear();

      return *this;
    }

    SplinePath2D &SplinePath2D::close()
    {
      if (points.size() > 2)
      {
        closed = true;

        if (points.front() == points.back())
        {
          points.pop_back();
        }
      }

      return *this;
    }

    SplinePath2D& SplinePath2D::add(const vector<glm::vec2> &newPoints)
    {
      if (!closed)
      {
        auto newSize = newPoints.size();

        if (newSize > 0)
        {
          points.reserve(points.size() + newSize);

          for (auto &point : newPoints)
          {
            add(point);
          }

          flushed = false;
        }
      }

      return *this;
    }

    SplinePath2D& SplinePath2D::add(const glm::vec2 &point)
    {
      if (!closed && (points.empty() || (point != points.back())))
      {
        points.emplace_back(point);
        flushed = false;
      }

      return *this;
    }

    SplinePath2D& SplinePath2D::transformPoints(const MatrixAffine &matrix)
    {
      for (auto &point : points)
      {
        point = matrix.transformPoint(point);
      }

      return *this;
    }

    bool SplinePath2D::flush()
    {
      if (!flushed)
      {
        auto size = points.size();

        if (size > 2)
        {
          begin();

          switch (type)
          {
            case TYPE_BSPLINE:
              gamma = GammaBSpline;
              break;

            case TYPE_CATMULL_ROM:
              gamma = GammaCatmullRom;
              break;
          }

          if (closed)
          {
            segment(points[size - 1], points[0], points[1], points[2]);
          }
          else
          {
            if (type == TYPE_BSPLINE)
            {
              segment(points[0], points[0], points[0], points[1]);
            }

            segment(points[0], points[0], points[1], points[2]);
          }

          for (auto i = 0; i < size - 3; i++)
          {
            segment(points[i], points[i + 1], points[i + 2], points[i + 3]);
          }

          if (closed)
          {
            segment(points[size - 3], points[size - 2], points[size - 1], points[0]);
            segment(points[size - 2], points[size - 1], points[0], points[1]);

            if (polyline.front() != polyline.back())
            {
              polyline.emplace_back(polyline.front());
            }
          }
          else
          {
            segment(points[size - 3], points[size - 2], points[size - 1], points[size - 1]);
            segment(points[size - 2], points[size - 1], points[size - 1], points[size - 1]);

            if (type == TYPE_BSPLINE)
            {
              segment(points[size - 1], points[size - 1], points[size - 1], points[size - 1]);
            }
          }

          flushed = true;
        }

        return flushed;
      }

      return false;
    }
  }
}
