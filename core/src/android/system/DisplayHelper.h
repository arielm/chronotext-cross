#pragma once

#include "system/DisplayHelperBase.h"
#include "system/SystemInfo.h"

namespace chr
{
  namespace display
  {
    class Helper : public HelperBase
    {
    public:
      static void setup(const system::InitInfo &initInfo);
      static void shutdown();
    };
  }
}
