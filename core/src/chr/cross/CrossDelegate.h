#pragma once

#include "chr/Platform.h"
#include "chr/Log.h"

#if defined(CHR_PLATFORM_DESKTOP)
  #define GLFW_INCLUDE_NONE
  #include <GLFW/glfw3.h>
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include <emscripten.h>
  #include <emscripten/html5.h>
#endif

#if defined(CHR_PLATFORM_IOS)
  #include "chr/ios/CrossDelegate.h"
#elif defined(CHR_PLATFORM_ANDROID)
  #include "chr/android/CrossDelegate.h"
#elif defined(CHR_PLATFORM_DESKTOP)
  #include "chr/desktop/CrossDelegate.h"
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include "chr/emscripten/CrossDelegate.h"
#endif
