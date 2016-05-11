#pragma once

#include "math/Utils.h"

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

      std::vector<glm::vec2> get() const
      {
        float h = sqrtf(3) * a / 2; // ALTITUDE FROM ANY SIDE

        std::vector<glm::vec2> output;
        output.reserve(3);

        output.emplace_back(0, -h * 2 / 3);
        output.emplace_back(-a / 2, h / 3);
        output.emplace_back(+a / 2, h / 3);

        return output;
      }

    protected:
      float a = 1;
    };
  }
}
