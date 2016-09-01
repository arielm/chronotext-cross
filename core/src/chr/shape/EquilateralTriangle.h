#pragma once

#include "chr/math/Utils.h"

namespace chr
{
  namespace shape
  {
    class EquilateralTriangle
    {
    public:
      EquilateralTriangle& setSideLength(float sideLength)
      {
        a = sideLength;
        return *this;
      }

      inline std::vector<glm::vec2> append(const glm::vec2 &xy) const { return append(xy.x, xy.y); }

      std::vector<glm::vec2> append(float x = 0, float y = 0) const
      {
        float h = sqrtf(3) * a / 2; // ALTITUDE FROM ANY SIDE

        std::vector<glm::vec2> output;
        output.reserve(3);

        output.emplace_back(x, y - h * 2 / 3);
        output.emplace_back(x - a / 2, y + h / 3);
        output.emplace_back(x + a / 2, y + h / 3);

        return output;
      }

    protected:
      float a = 1;
    };
  }
}
