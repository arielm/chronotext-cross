
#include "CrossSketch.h"

#if defined(CHR_PLATFORM_DESKTOP) || defined(CHR_PLATFORM_EMSCRIPTEN)
  #include "cross/CrossDelegate.h"
#endif

using namespace std;

namespace chr
{
  #if defined(CHR_PLATFORM_DESKTOP)
    void CrossSketch::run(int width, int height, int aaSamples, int depthBits)
    {
        CrossDelegate delegate;
        delegate.run(width, height, aaSamples, depthBits);
    }
  #elif defined(CHR_PLATFORM_EMSCRIPTEN)
    void CrossSketch::run(int aaSamples, int depthBits)
    {
      CrossDelegate delegate;
      delegate.run(aaSamples, depthBits);
    }
  #endif

  double CrossSketch::getElapsedSeconds()
  {
    return timer.getSeconds(); // OUR FrameClock IS NOT SUITED BECAUSE IT PROVIDES A UNIQUE TIME-SAMPLE PER FRAME
  }

  int CrossSketch::getElapsedFrames()
  {
    return frameCount;
  }

  const WindowInfo& CrossSketch::getWindowInfo() const
  {
    return windowInfo;
  }

  FrameClock::Ref CrossSketch::clock() const
  {
    return _clock;
  }

  // ---

  void CrossSketch::performSetup(const WindowInfo &windowInfo)
  {
    this->windowInfo = windowInfo;
    forceResize = true;

    setup();
  }

  void CrossSketch::performResize(const glm::vec2 &size)
  {
    if (forceResize || (size != windowInfo.size))
    {
      windowInfo.size = size;
      forceResize = false;

      resize();
    }
  }

  void CrossSketch::performStart(StartReason reason)
  {
    frameCount = 0;

    timer.start();
    _clock->start();

    start(reason);
  }

  void CrossSketch::performStop(StopReason reason)
  {
    timer.stop();
   _clock->stop();

    stop(reason);
  }

  void CrossSketch::performUpdate()
  {
    update();

    frameCount++;
  }
}
