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
  }
}
