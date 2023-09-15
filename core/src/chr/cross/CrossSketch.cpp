#include "chr/cross/CrossSketch.h"

#if defined(CHR_PLATFORM_DESKTOP) || defined(CHR_PLATFORM_EMSCRIPTEN)
  #include "chr/cross/CrossDelegate.h"
#endif

#if defined(CHR_PLATFORM_DESKTOP)
  #define STB_IMAGE_IMPLEMENTATION
  #define STB_IMAGE_WRITE_IMPLEMENTATION
  #define STB_IMAGE_RESIZE_IMPLEMENTATION
  #include "stb_image.h"
  #include "stb_image_write.h"
  #include "stb_image_resize.h"
#endif

#include <iomanip>

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

  void CrossSketch::performResize(const glm::vec2 &size, float safeAreaInsetsTop, float safeAreaInsetsBottom)
  {
    if (forceResize || (size != windowInfo.size))
    {
      windowInfo.size = size;
      windowInfo.safeAreaInsetsTop = safeAreaInsetsTop;
      windowInfo.safeAreaInsetsBottom = safeAreaInsetsBottom;
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

  #if defined(CHR_PLATFORM_DESKTOP)
    void CrossSketch::grabScreen(const fs::path &destinationDirectory)
    {
      stringstream stream;
      stream << std::setfill('0') << std::setw(6) << grabbedFrameCount++;
      fs::path filePath = destinationDirectory / ("IMG_" + stream.str() + ".png");

      performGrabScreen(filePath);
    }

    void CrossSketch::performGrabScreen(const fs::path &filePath)
    {
      float contentScale = getDisplayInfo().contentScale;
      int w = contentScale * windowInfo.width;
      int h = contentScale * windowInfo.height;

      uint8_t *pixels = new uint8_t[w * h * 3];
      glPixelStorei(GL_PACK_ALIGNMENT, 1);
      glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)pixels);

      stbi_flip_vertically_on_write(1);

      if (stbi_write_png(filePath.string().data(), w, h, 3, pixels, 3 * w))
      {
        LOGI << "IMAGE WRITTEN: " << filePath.string() << endl;
      }
      else
      {
        LOGE << "ERROR WRITING IMAGE: " << filePath.string() << endl;
      }

      delete[] pixels;
    }
  #endif
}
