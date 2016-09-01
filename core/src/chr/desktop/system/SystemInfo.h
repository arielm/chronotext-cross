#pragma once

#include "chr/system/SystemInfoBase.h"

struct GLFWwindow;

namespace chr
{
  namespace system
  {
    struct InitInfo
    {
      WindowInfo windowInfo;
      GLFWwindow* window = nullptr;

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
