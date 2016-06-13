#pragma once

#include "glm.h"
#include "math/Utils.h"

namespace chr
{
  namespace math
  {
    class Plane
    {
    public:
      glm::vec3 normal;
      float distance;

      Plane() = default;

      Plane(const glm::vec3 &point, const glm::vec3 &normal)
      {
        distance = glm::dot(normal, point);
      }

      Plane(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2)
      {
        normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));
        distance = glm::dot(normal, p0);
      }
    };
  }
}
