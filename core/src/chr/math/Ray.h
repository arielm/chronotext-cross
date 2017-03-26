#pragma once

#include "chr/glm.h"
#include "chr/math/Utils.h"

namespace chr
{
  namespace math
  {
    class Ray
    {
    protected:
      static constexpr float EPSILON = 0.000001f;

    public:
      glm::vec3 origin;
      glm::vec3 direction;

      Ray() = default;

      Ray(const glm::vec3 &origin, const glm::vec3 &direction)
      :
      origin(origin),
      direction(direction)
      {}

      /*
       * SOURCE-CODE REFERENCES FOR THE triangleIntersection() METHOD:
       *
       * 1) Möller–Trumbore intersection algorithm
       *    https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
       *
       * 2) CINDER'S IMPLEMENTATION
       *    https://github.com/cinder/Cinder/blob/master/src/cinder/Ray.cpp
       */

      std::pair<bool, float> triangleIntersection(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, bool culling = false, int frontFace = CCW) const
      {
        // find vectors for two edges sharing v1
        glm::vec3 e1(v2 - v1);
        glm::vec3 e2(v3 - v1);

        // begin calculating determinant - also used to calculate u parameter
        glm::vec3 P(glm::cross(direction, e2));

        // if determinant is near zero, ray lies in plane of triangle
        float det = glm::dot(e1, P);

        if (culling)
        {
          if (frontFace == CW)
          {
            if (det < EPSILON)
            {
              return std::make_pair(false, 0);
            }
          }
          else if (det > -EPSILON)
          {
            return std::make_pair(false, 0);
          }
        }
        else if (fabsf(det) < EPSILON)
        {
          return std::make_pair(false, 0);
        }

        // calculate distance from v1 to ray origin
        glm::vec3 T(origin - v1);

        // calculate u parameter and test bound
        float u = glm::dot(T, P) / det;

        // the intersection lies outside of the triangle
        if ((u < 0) || (u > 1))
        {
          return std::make_pair(false, 0);
        }

        glm::vec3 Q(glm::cross(T, e1));

        // calculate v parameter and test bound
        float v = glm::dot(direction, Q) / det;

        // the intersection lies outside of the triangle
        if ((v < 0) || (u + v  > 1))
        {
          return std::make_pair(false, 0);
        }

        return std::make_pair(true, glm::dot(e2, Q) / det);
      }

      std::pair<bool, float> planeIntersection(const glm::vec3 &planeOrigin, const glm::vec3 &planeNormal) const
      {
        float denom = glm::dot(planeNormal, direction);

        if (fabsf(denom) < EPSILON)
        {
          return std::make_pair(false, 0); // RAY IS PARALLEL TO PLANE
        }

        return std::make_pair(true, glm::dot(planeNormal, planeOrigin - origin) / denom); // IF < 0: PLANE IS BEHIND RAY'S ORIGIN
      }
    };
  }
}
