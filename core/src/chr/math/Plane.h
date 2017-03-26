/*
 * MATHS BASED ON https://github.com/cinder/Cinder/blob/master/src/cinder/Plane.cpp
 */

#pragma once

#include "chr/glm.h"
#include "chr/math/Utils.h"

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
      Plane(const glm::vec3 &point, const glm::vec3 &normal);
      Plane(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2);
      Plane(float a, float b, float c, float d);

      void set(const glm::vec3 &point, const glm::vec3 &normal);
      void set(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2);
      void set(float a, float b, float c, float d);

      float signedDistance(const glm::vec3 &point) const;
    };
  }
}
