#pragma once

#include "math/Utils.h"

/*
 * THE DEFAULT INNER-RADIUS-RATIO IS EQUAL TO (3 - sqrt(5)) / 2
 * AS WELL AS 1 / (phi ^ 2), WHERE phi IS THE GOLDEN RATIO
 */

namespace chr
{
  namespace shape
  {
    class FivePointedStar
    {
    public:
      FivePointedStar& setOuterRadius(float radius)
      {
        outerRadius = radius;
        return *this;
      }

      FivePointedStar& setInnerRadiusRatio(float ratio)
      {
        innerRadiusRatio = ratio;
        return *this;
      }

      std::vector<glm::vec2> get() const
      {
        float innerRadius = outerRadius * innerRadiusRatio;

        std::vector<glm::vec2> output;
        output.reserve(10);

        for (int i = 0; i < 5; i++)
        {
          /*
           * DRAWING IN CCW ORDER
           */

          float outerAngle = -i * TWO_PI / 5.0f;
          float innerAngle = outerAngle - TWO_PI / 10.0f;

          output.emplace_back(+sinf(outerAngle) * outerRadius, -cosf(outerAngle) * outerRadius);
          output.emplace_back(+sinf(innerAngle) * innerRadius, -cosf(innerAngle) * innerRadius);
        }

        return output;
      }

    protected:
      float outerRadius = 1;
      float innerRadiusRatio = 0.38196601125f;
    };
  }
}
