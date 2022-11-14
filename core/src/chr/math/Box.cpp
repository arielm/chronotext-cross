#include "chr/math/Box.h"

namespace chr
{
  namespace math
  {
    Box::Box(const glm::vec3 min, const glm::vec3 max)
    :
    min(min),
    max(max)
    {}

    Box::Box(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax)
    :
    min(xmin, ymin, zmin),
    max(xmax, ymax, zmax)
    {}

    float Box::width() const
    {
      return fabsf(max.x - min.x);
    }

    float Box::height() const
    {
      return fabsf(max.y - min.y);
    }

    float Box::depth() const
    {
      return fabsf(max.z - min.z);
    }

    glm::vec3 Box::center() const
    {
      return glm::vec3(min + max) * 0.5f;
    }

    void Box::include(const Box &box)
    {
      min = glm::min(min, box.min);
      max = glm::max(max, box.max);
    }
  }
}
