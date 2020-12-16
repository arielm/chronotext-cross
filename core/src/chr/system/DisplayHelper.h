#pragma once

#include "chr/Platform.h"
#include "chr/Log.h"

#if defined(CHR_PLATFORM_IOS)
  #include "chr/ios/system/DisplayHelper.h"
#elif defined(CHR_PLATFORM_ANDROID)
  #include "chr/android/system/DisplayHelper.h"
#elif defined(CHR_PLATFORM_GLFW)
  #include "chr/glfw/system/DisplayHelper.h"
#elif defined(CHR_PLATFORM_SDL)
  #include "chr/sdl/system/DisplayHelper.h"
#elif defined(CHR_PLATFORM_WIN)
  #include "chr/win/system/DisplayHelper.h"
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include "chr/emscripten/system/DisplayHelper.h"
#endif

namespace chr
{
  typedef display::Helper DisplayHelper;
}
