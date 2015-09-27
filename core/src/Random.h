#pragma once

#include <random>

/*
 * BASED ON https://github.com/cinder/Cinder/blob/master/include/cinder/Rand.h
 */

namespace chr
{
  class Random
  {
  public:
      Random();
      Random(uint32_t seed);

      void seed( uint32_t seedValue );
  
      bool nextBool();
      int32_t nextInt();
      uint32_t nextUint();
      int32_t nextInt(int32_t v);
      uint32_t nextUint(uint32_t v);
      int32_t nextInt(int32_t a, int32_t b);
      float nextFloat();
      float nextFloat(float v);
      float nextFloat(float a, float b);

  protected:
      std::mt19937 base;
      std::uniform_real_distribution<float> floatGenerator;
  };
}
