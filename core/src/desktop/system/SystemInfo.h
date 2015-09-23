/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "system/SystemInfoBase.h"

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
