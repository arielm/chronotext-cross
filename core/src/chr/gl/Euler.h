/*
 * Based on:
 * - https://github.com/mrdoob/three.js/blob/master/src/math/Euler.js
 * - https://github.com/mrdoob/three.js/blob/master/src/math/Quaternion.js
 */

#pragma once

#include "chr/gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    class Euler
    {
    public:
      static glm::vec3 fromRotationMatrix(const Matrix &m); // Assumes the upper 3x3 of m is a pure rotation matrix (i.e. unscaled)
      static glm::vec3 fromQuaternion(const glm::quat &q);
      static glm::quat toQuaternion(const glm::vec3 &euler);
    };
  }
}
