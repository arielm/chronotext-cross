#pragma once

#include "chr/system/SystemInfoBase.h"

#include <jni.h>

namespace chr
{
  namespace system
  {
    struct InitInfo
    {
      jobject androidContext = nullptr;
      jobject androidDisplay = nullptr;
      
      glm::vec2 displaySize {};
      float displayDensity = 0;
    };
    
    class Info : public InfoBase
    {
    public:
      int sdkVersion = 0;
      std::string model;
      std::string manufacturer;
    };
  }
}
