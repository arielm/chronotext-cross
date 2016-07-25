#include "math/Utils.h"

namespace chr
{
  namespace math
  {
    float boundf(float value, float range)
    {
      float bound = fmodf(value, range);
      return (bound < 0) ? (bound + range) : bound;
    }
    
    int bound(int value, int range)
    {
      int bound = value % range;
      return (bound < 0) ? (bound + range) : bound;
    }

    float constrainf(float value, float min, float max)
    {
      return (value < min) ? min : ((value > max) ? max : value);
    }

    int constrain(int value, int min, int max)
    {
      return (value < min) ? min : ((value > max) ? max : value);
    }
    
    float ease(float t)
    {
      return (t * t * (3 - 2 * t));
    }

    float easeIn(float t)
    {
      return (t < 0.5f) ? ease(t) : t;
    }

    float easeOut(float t)
    {
      return (t > 0.5f) ? ease(t) : t;
    }
    
    int nextPowerOfTwo(int x)
    {
      int result = 1;
      
      while (result < x)
      {
        result <<= 1;
      }
      
      return result;
    }
    
    bool isPowerOfTwo(int x)
    {
      return (x > 0) && !(x & (x - 1));
    }

    glm::vec3 transformPoint(const glm::mat4 &matrix, const glm::vec2 &input)
    {
      float x = matrix[0][0] * input.x + matrix[1][0] * input.y + matrix[3][0];
      float y = matrix[0][1] * input.x + matrix[1][1] * input.y + matrix[3][1];
      float z = matrix[0][2] * input.x + matrix[1][2] * input.y + matrix[3][2];

      return glm::vec3(x, y, z);
    }

    glm::vec3 transformPoint(const glm::mat4 &matrix, const glm::vec3 &input)
    {
      float x = matrix[0][0] * input.x + matrix[1][0] * input.y + matrix[2][0] * input.z + matrix[3][0];
      float y = matrix[0][1] * input.x + matrix[1][1] * input.y + matrix[2][1] * input.z + matrix[3][1];
      float z = matrix[0][2] * input.x + matrix[1][2] * input.y + matrix[2][2] * input.z + matrix[3][2];

      return glm::vec3(x, y, z);
    }

    glm::quat quatFromEuler(float eulerX, float eulerY, float eulerZ)
    {
      glm::quat qx = glm::angleAxis(eulerX, glm::vec3(1, 0, 0));
      glm::quat qy = glm::angleAxis(eulerY, glm::vec3(0, 1, 0));
      glm::quat qz = glm::angleAxis(eulerZ, glm::vec3(0, 0, 1));

      return qx * qy * qz;
    }
  }
}
