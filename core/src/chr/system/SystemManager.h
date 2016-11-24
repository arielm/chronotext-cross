#pragma once

#include "chr/Platform.h"
#include "chr/Log.h"

#if defined(CHR_PLATFORM_IOS)
  #include "chr/ios/system/SystemManager.h"
#elif defined(CHR_PLATFORM_ANDROID)
  #include "chr/android/system/SystemManager.h"
#elif defined(CHR_PLATFORM_OSX)
  #include "chr/osx/system/SystemManager.h"
#elif defined(CHR_PLATFORM_WIN)
  #include "chr/win/system/SystemManager.h"
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include "chr/emscripten/system/SystemManager.h"
#endif

namespace chr
{
  typedef system::Manager SystemManager;
}
