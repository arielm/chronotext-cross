#pragma once

#include "chr/Platform.h"
#include "chr/Log.h"

#if defined(CHR_PLATFORM_IOS)
  #include "chr/ios/system/SystemInfo.h"
#elif defined(CHR_PLATFORM_ANDROID)
  #include "chr/android/system/SystemInfo.h"
#elif defined(CHR_PLATFORM_DESKTOP)
  #include "chr/desktop/system/SystemInfo.h"
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include "chr/emscripten/system/SystemInfo.h"
#endif

namespace chr
{
  typedef system::Info SystemInfo;
}
