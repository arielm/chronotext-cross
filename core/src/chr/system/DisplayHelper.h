#pragma once

#include "chr/Platform.h"
#include "chr/Log.h"

#if defined(CHR_PLATFORM_IOS)
  #include "chr/ios/system/DisplayHelper.h"
#elif defined(CHR_PLATFORM_ANDROID)
  #include "chr/android/system/DisplayHelper.h"
#elif defined(CHR_PLATFORM_DESKTOP)
  #include "chr/desktop/system/DisplayHelper.h"
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include "chr/emscripten/system/DisplayHelper.h"
#endif

namespace chr
{
  typedef display::Helper DisplayHelper;
}
