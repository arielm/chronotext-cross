#pragma once

#include "chr/system/DisplayHelperBase.h"
#include "chr/system/SystemInfo.h"

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
