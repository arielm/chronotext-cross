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
    
    float ease(float t)
    {
      return (t * t * (3 - 2 * t));
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
  }
}
