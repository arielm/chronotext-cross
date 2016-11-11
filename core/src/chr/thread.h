#pragma once

#include "chr/Platform.h"

#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

#if defined(CHR_PLATFORM_MINGW)
  #include "mingw.thread.h"
  #include "mingw.mutex.h"
  #include "mingw.condition_variable.h"
#endif
