#include "Plane.h"

namespace chr
{
  namespace math
  {
    Plane::Plane(const glm::vec3 &point, const glm::vec3 &normal)
    {
      set(point, normal);
    }

    Plane::Plane(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2)
    {
      set(p0, p1, p2);
    }

    Plane::Plane(float a, float b, float c, float d)
    {
      set(a, b, c, d);
    }

    void Plane::set(const glm::vec3 &point, const glm::vec3 &normal)
    {
      this->normal = glm::normalize(normal);
      distance = glm::dot(this->normal, point);
    }

    void Plane::set(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2)
    {
      normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));
      distance = glm::dot(normal, p0);
    }

    void Plane::set(float a, float b, float c, float d)
    {
      glm::vec3 normal(a, b, c);

      this->normal = normalize(normal);
      distance = d / glm::length(normal);
    }

    float Plane::signedDistance(const glm::vec3 &point) const
    {
      return glm::dot(normal, point) - distance;
    }
  }
}
