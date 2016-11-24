#include "chr/Timer.h"

#if defined(CHR_PLATFORM_EMSCRIPTEN)
  #include <emscripten.h>
#else
  #include <chrono>
#endif

namespace chr
{
  Timer::Timer(bool startOnConstruction)
  {
    if (startOnConstruction)
    {
      start();
    }
  }

  void Timer::start()
  {
    startTime = getCurrent();
    stopped = false;
  }

  void Timer::stop()
  {
    if (!stopped)
    {
      endTime = getCurrent();
      stopped = true;
    }
  }

  double Timer::getSeconds()
  {
    if (stopped)
    {
      return endTime - startTime;
    }
    else
    {
      return getCurrent() - startTime;
    }
  }

  bool Timer::isStopped() const
  {
    return stopped;
  }

#if defined(CHR_PLATFORM_EMSCRIPTEN)
  double Timer::getCurrent()
  {
    return emscripten_get_now() / 1000.0;
  }
#else
  double Timer::getCurrent()
  {
    auto now = std::chrono::steady_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(now).count() / 1000000.0;
  }
#endif
}
