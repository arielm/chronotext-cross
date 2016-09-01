#pragma once

#include "chr/cross/CrossDelegate.h"
#include "chr/system/SystemManager.h"

namespace chr
{
  CrossSketch* createSketch();
  
  // ---

  extern CrossDelegate& delegate();
  extern SystemManager& systemManager();

  template <class T>
  inline T& checkedReference(T *instance)
  {
  assert(instance);
  return *instance;
  }
}
