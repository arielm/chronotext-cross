#include "chr/Random.h"

using namespace std;

namespace chr
{
  Random::Random()
  :
  base(214u)
  {}
  
  Random::Random(uint32_t seed)
  :
  base(seed)
  {}

  void Random::seed(uint32_t seedValue)
  {
    base = mt19937(seedValue);
  }
  
  bool Random::nextBool()
  {
    return base() & 1;
  }
  
  int32_t Random::nextInt()
  {
    return base();
  }

  uint32_t Random::nextUint()
  {
    return base();
  }
  
  int32_t Random::nextInt(int32_t v)
  {
    return (v <= 0) ? 0 : (base() % v);
  }

  uint32_t Random::nextUint(uint32_t v)
  {
    return (v == 0) ? 0 : (base() % v);
  }
  
  int32_t Random::nextInt(int32_t a, int32_t b)
  {
    return nextInt(b - a ) + a;
  }
  
  float Random::nextFloat()
  {
    return floatGenerator(base);
  }
  
  float Random::nextFloat(float v)
  {
    return floatGenerator(base) * v;
  }
  
  float Random::nextFloat(float a, float b)
  {
    return floatGenerator(base) * (b - a) + a;
  }
}
