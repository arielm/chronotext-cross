#pragma once

#include "chr/Platform.h"
#include "chr/Log.h"

#if defined(CHR_PLATFORM_GLFW)
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
#elif defined(CHR_PLATFORM_GLFW)
  #include "chr/glfw/CrossDelegate.h"
#elif defined(CHR_PLATFORM_WIN)
  #include "chr/win/CrossDelegate.h"
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include "chr/emscripten/CrossDelegate.h"
#endif
