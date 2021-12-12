#pragma once

#include "chr/math/Plane.h"
#include "chr/math/Box.h"

namespace chr
{
  namespace math
  {
    class Frustum
    {
    public:
      enum
      {
        PLANE_NEAR,
        PLANE_FAR,
        PLANE_LEFT,
        PLANE_RIGHT,
        PLANE_TOP,
        PLANE_BOTTOM
      };

      Plane planes[6];

      Frustum() = default;

      Frustum(const glm::mat4 &mat);
      void set(const glm::mat4 &mat);

      bool contains(const glm::vec3 &point) const;
      bool containsOrIntersectsWith(const Box &box) const;
    };
  }
}
