/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "Platform.h"
#include "Log.h"

#if defined(CHR_PLATFORM_DESKTOP)
  #define GLFW_INCLUDE_NONE
  #include <GLFW/glfw3.h>
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include <emscripten.h>
  #include <emscripten/html5.h>
#endif

#if defined(CHR_PLATFORM_IOS)
  #include "ios/CrossDelegate.h"
#elif defined(CHR_PLATFORM_ANDROID)
  #include "android/CrossDelegate.h"
#elif defined(CHR_PLATFORM_DESKTOP)
  #include "desktop/CrossDelegate.h"
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include "emscripten/CrossDelegate.h"
#endif
