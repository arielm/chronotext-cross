#pragma once

#include "chr/math/Utils.h"

namespace chr
{
  namespace shape
  {
    class Circle
    {
    public:
      Circle& setRadius(float radius)
      {
        r = radius;
        return *this;
      }

      Circle& setArc(float a1, float a2)
      {
        this->a1 = a1;
        this->a2 = a2;

        return *this;
      }

      Circle& setSegmentLength(float length)
      {
        segmentLength = length;
        return *this;
      }

      inline std::vector<glm::vec2> append(const glm::vec2 &xy) const { return append(xy.x, xy.y); }

      std::vector<glm::vec2> append(float x = 0, float y = 0) const
      {
        float aa = fabsf(a2 - a1);
        int n = ceilf(aa * r / segmentLength) + 1;

        std::vector<glm::vec2> output;
        output.reserve(n);

        for (int i = 0; i < n; i++)
        {
          float d = fminf(aa, i * segmentLength / r);
          output.emplace_back(x + sinf(a1 - d) * r, y - cosf(a1 - d) * r);
        }

        return output;
      }

    protected:
      float r = 1;
      float a1 = 0;
      float a2 = TWO_PI;
      float segmentLength = 8;
    };
  }
}
