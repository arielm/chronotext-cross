
#pragma once

#include "glm.h"

#include <iostream>

namespace chr
{
  namespace math
  {
    class Box
    {
    public:
    	glm::vec3 min;
    	glm::vec3 max;

      Box() = default;
      Box(const glm::vec3 min, const glm::vec3 max);
      Box(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);

      float width() const;
      float height() const;
      float depth() const;

      glm::vec3 center() const;

      friend std::ostream& operator<<(std::ostream &stream, const Box &box)
      {
        stream
          << "{"
          << "min: " << box.min.x << ", " << box.min.y << ", " << box.min.z
          << ", max: " << box.max.x << ", " << box.max.y << ", " << box.max.z
          << "}";

        return stream;
      }
    };
  }
}
