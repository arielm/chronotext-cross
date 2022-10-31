#include "Euler.h"

namespace chr
{
  namespace gl
  {
    glm::vec3 Euler::fromRotationMatrix(const Matrix &m)
    {
      glm::vec3 euler;

      euler.y = asinf(math::constrainf(m.m02, -1, 1));

      if (fabsf(m.m02) < 0.9999999f)
      {
        euler.x = atan2f(-m.m12, m.m22);
        euler.z = atan2f(-m.m01, m.m00);
      }
      else
      {
        euler.x = atan2f(m.m21, m.m11);
        euler.z = 0;
      }

      return euler;
    }

    glm::vec3 Euler::fromQuaternion(const glm::quat &q)
    {
      return fromRotationMatrix(Matrix(q));
    }

    glm::quat Euler::toQuaternion(const glm::vec3 &euler)
    {
      float c1 = cosf(euler.x / 2);
      float c2 = cosf(euler.y / 2);
      float c3 = cosf(euler.z / 2);

      float s1 = sinf(euler.x / 2);
      float s2 = sinf(euler.y / 2);
      float s3 = sinf(euler.z / 2);

      glm::quat q;

      q.x = s1 * c2 * c3 + c1 * s2 * s3;
      q.y = c1 * s2 * c3 - s1 * c2 * s3;
      q.z = c1 * c2 * s3 + s1 * s2 * c3;
      q.w = c1 * c2 * c3 - s1 * s2 * s3;

      return q;
    }
  }
}
