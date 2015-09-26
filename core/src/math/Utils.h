/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <cmath>
#include <limits>

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

namespace chr
{
  static constexpr float D2R (M_PI / 180.0);
  static constexpr float R2D (180.0 / M_PI);
  
  static constexpr float PI (M_PI);
  static constexpr float TWO_PI (M_PI * 2.0);
  static constexpr float HALF_PI (M_PI * 0.5);

  namespace math
  {
    inline float boundf(float value, float range)
    {
      float bound = fmodf(value, range);
      return (bound < 0) ? (bound + range) : bound;
    }
    
    inline int bound(int value, int range)
    {
      int bound = value % range;
      return (bound < 0) ? (bound + range) : bound;
    }
    
    /*
     * S-SHAPED CROSS-FADE CURVE: 3 * (t ^ 2) - 2 * (t ^ 3)
     */
    inline float ease(float t)
    {
      return (t * t * (3 - 2 * t));
    }
    
    inline int nextPowerOfTwo(int x)
    {
      int result = 1;
      
      while (result < x)
      {
        result <<= 1;
      }
      
      return result;
    }
    
    inline bool isPowerOfTwo(int x)
    {
      return (x > 0) && !(x & (x - 1));
    }
  }
}
