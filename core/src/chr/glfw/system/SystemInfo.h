#pragma once

#include "chr/system/SystemInfoBase.h"

namespace chr
{
  namespace system
  {
    struct InitInfo
    {
      WindowInfo windowInfo;

      InitInfo() = default;

      InitInfo(const WindowInfo &windowInfo)
      :
      windowInfo(windowInfo)
      {}
    };

    class Info : public InfoBase
    {};
  }
}
