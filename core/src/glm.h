#pragma once

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/norm.hpp>

#include <tuple>

namespace glm
{
  template <typename T, precision P>
  bool operator<(const tvec2<T, P> &lhs, const tvec2<T, P> &rhs)
  {
    return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
  }
};
