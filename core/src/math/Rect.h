
#pragma once

#include "glm.h"

namespace chr
{
  namespace math
  {
    class Rectf
    {
    public:
    	float x1;
    	float y1;
    	float x2;
    	float y2;

      Rectf() = default;
      Rectf(float left, float top, float width, float height);
      Rectf(const glm::vec2 &upperLeft, const glm::vec2 &lowerRight);

      float width() const;
      float height() const;
      glm::vec2 size() const;

      glm::vec2 x1y1() const;
      glm::vec2 x2y1() const;
      glm::vec2 x2y2() const;
      glm::vec2 x1y2() const;
    };
  }
}
