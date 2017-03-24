#pragma once

#include "chr/glm.h"

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

      Rectf& operator +=(const glm::vec2 &offset);
      Rectf& operator -=(const glm::vec2 &offset);

      float width() const;
      float height() const;
      glm::vec2 size() const;

      glm::vec2 x1y1() const;
      glm::vec2 x2y1() const;
      glm::vec2 x2y2() const;
      glm::vec2 x1y2() const;

      bool isNull() const;

      bool contains(float x, float y) const;
      inline bool contains(const glm::vec2 &point) const { return contains(point.x, point.y); }

      void inflate(const glm::vec2 amount);
      Rectf inflated(const glm::vec2 amount) const;

      glm::vec2 center() const;
      float distance(const glm::vec2 &point) const;
    };
  }
}
