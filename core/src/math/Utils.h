#pragma once

#include <cmath>
#include <limits>

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

namespace chr
{
  enum
  {
    CULLING,
    NO_CULLING
  };

  enum
  {
    CW,
    CCW
  };

  static constexpr float D2R = float(M_PI / 180.0);
  static constexpr float R2D = float(180.0 / M_PI);
  
  static constexpr float PI = float(M_PI);
  static constexpr float TWO_PI = float(M_PI * 2.0);
  static constexpr float HALF_PI = float(M_PI * 0.5);

  namespace math
  {
    float boundf(float value, float range);
    int bound(int value, int range);

    float constrainf(float value, float min, float max);
    int constrain(int value, int min, float max);

    /*
     * S-SHAPED CROSS-FADE CURVE: 3 * (t ^ 2) - 2 * (t ^ 3)
     */
    float ease(float t);
    
    int nextPowerOfTwo(int x);
    bool isPowerOfTwo(int x);
  }
}
