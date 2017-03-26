#pragma once

#include "chr/math/Plane.h"

namespace chr
{
  namespace math
  {
    class Frustum
    {
    public:
      Frustum() = default;
      Frustum(const glm::mat4 &mat); // TAKES A MODEL-VIEW-PROJECTION MATRIX

      void set(const glm::mat4 &mat); // TAKES A MODEL-VIEW-PROJECTION MATRIX
      bool contains(const glm::vec3 &point) const;

    protected:
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
    };
  }
}
