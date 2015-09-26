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
    float boundf(float value, float range);
    int bound(int value, int range);
    
    /*
     * S-SHAPED CROSS-FADE CURVE: 3 * (t ^ 2) - 2 * (t ^ 3)
     */
    float ease(float t);
    
    int nextPowerOfTwo(int x);
    bool isPowerOfTwo(int x);
  }
}
