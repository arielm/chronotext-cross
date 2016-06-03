#include "path/Path.h"

using namespace std;

namespace chr
{
  namespace path
  {
    Path::Path(vector<glm::vec2> &&polyline)
    :
    ASPC(std::forward<vector<glm::vec2>>(polyline))
    {}

    const vector <glm::vec2> &Path::getPolyline() const
    {
      return polyline;
    }

    size_t Path::size() const
    {
      return polyline.size();
    }

    bool Path::empty() const
    {
      return polyline.empty();
    }

    bool Path::isClosed() const
    {
      return (polyline.size() > 2) && (current == polyline.front());
    }

    Path &Path::setSamplingTolerance(float tolerance)
    {
      samplingTolerance = tolerance;
      return *this;
    }

    Path &Path::clear()
    {
      begin();
      current = {numeric_limits<float>::quiet_NaN(), numeric_limits<float>::quiet_NaN()};

      return *this;
    }

    Path &Path::close()
    {
      if ((polyline.size() > 2) && (current != polyline.front()))
      {
        polyline.emplace_back(polyline.front());
        current = polyline.front();
      }

      return *this;
    }

    Path &Path::moveTo(const glm::vec2 &p)
    {
      begin();
      polyline.emplace_back(p);
      current = p;

      return *this;
    }

    Path &Path::lineTo(const glm::vec2 &p)
    {
      if (current != p)
      {
        polyline.emplace_back(p);
        current = p;
      }

      return *this;
    }

    Path &Path::quadraticCurveTo(const glm::vec2 &p1, const glm::vec2 &p2)
    {
      gamma = GammaQuadratic;
      segment(current, p1, p2);

      polyline.emplace_back(p2);
      current = p2;

      return *this;
    }

    Path &Path::bezierCurveTo(const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &p3)
    {
      gamma = GammaBezier;
      segment(current, p1, p2, p3);

      polyline.emplace_back(p3);
      current = p3;

      return *this;
    }

    Path& Path::cleanup()
    {
      size_t lastIndex = 0;
      size_t size = polyline.size();

      for (auto index = 0; index < size; ++index, ++lastIndex)
      {
        while (polyline[index] == polyline[index + 1])
        {
          ++index;
        }

        if (index >= size)
        {
          break;
        }

        polyline[lastIndex] = polyline[index];
      }

      polyline.resize(lastIndex);
      return *this;
    }
  }
}
