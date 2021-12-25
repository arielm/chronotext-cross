#pragma once

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/ext.hpp>

#include <tuple>
#include <iostream>

namespace glm
{
  template <typename T, precision P>
  bool operator<(const tvec2<T, P> &lhs, const tvec2<T, P> &rhs)
  {
    return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
  }

  template <typename T, precision P>
  bool operator<(const tvec3<T, P> &lhs, const tvec3<T, P> &rhs)
  {
    return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
  }

  // ---

  template <typename T, precision P>
  std::ostream& operator<<(std::ostream &lhs, const tvec2<T, P> &rhs)
  {
    lhs
      << "["
      << rhs.x << ", " << rhs.y
      << "]";

    return lhs;
  }

  template <typename T, precision P>
  std::ostream& operator<<(std::ostream &lhs, const tvec3<T, P> &rhs)
  {
    lhs
      << "["
      << rhs.x << ", " << rhs.y << ", " << rhs.z
      << "]";

    return lhs;
  }

  template <typename T, precision P>
  std::ostream& operator<<(std::ostream &lhs, const tquat<T, P> &rhs)
  {
    lhs
      << "["
      << rhs.w << ", " << rhs.x << ", " << rhs.y << ", " << rhs.z
      << "]";

    return lhs;
  };
}
