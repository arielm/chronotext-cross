#include "path/SplinePath.h"

using namespace std;

namespace chr
{
  namespace path
  {
    SplinePath::SplinePath(int capacity)
    {
      if (capacity > 0)
      {
        points.reserve(capacity);
      }
    }

    SplinePath::SplinePath(const std::vector <glm::vec2> &points)
    {
      add(points);
    }

    const vector<glm::vec2>& SplinePath::getPolyline()
    {
      flush();
      return polyline;
    }

    const vector<glm::vec2>& SplinePath::getPoints() const
    {
      return points;
    }

    size_t SplinePath::size() const
    {
      return points.size();
    }

    bool SplinePath::empty() const
    {
      return points.empty();
    }

    bool SplinePath::isClosed() const
    {
      return closed;
    }

    SplinePath &SplinePath::setSamplingTolerance(float tolerance)
    {
      samplingTolerance = tolerance;
      return *this;
    }

    SplinePath &SplinePath::setType(Type type)
    {
      this->type = type;
      return *this;
    }

    SplinePath &SplinePath::clear()
    {
      flushed = false;

      points.clear();
      polyline.clear();

      return *this;
    }

    SplinePath &SplinePath::close()
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

    SplinePath& SplinePath::add(const vector<glm::vec2> &newPoints)
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

    SplinePath& SplinePath::add(const glm::vec2 &point)
    {
      if (!closed && (points.empty() || (point != points.back())))
      {
        points.emplace_back(point);
        flushed = false;
      }

      return *this;
    }

    bool SplinePath::flush()
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
