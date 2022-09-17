#pragma once

#include "chr/glm.h"
#include "chr/Timer.h"
#include "chr/cross/AccelEvent.h"
#include "chr/cross/WindowInfo.h"
#include "chr/system/DisplayHelper.h"
#include "chr/time/FrameClock.h"
#include "chr/utils/Utils.h"
#include "chr/math/Utils.h"
#include "chr/gl/Utils.h"

namespace chr
{
  class CrossSketch
  {
  public:
    enum StartReason
    {
      START_REASON_VIEW_SHOWN = 1,
      START_REASON_APP_RESUMED
    };

    enum StopReason
    {
      STOP_REASON_VIEW_HIDDEN = 1,
      STOP_REASON_APP_PAUSED
    };

    enum
    {
      EVENT_RESUMED = 1,
      EVENT_SHOWN,
      EVENT_PAUSED,
      EVENT_HIDDEN,
      EVENT_FOREGROUND,
      EVENT_BACKGROUND,
      EVENT_MEMORY_WARNING,
      EVENT_CONTEXT_LOST,
      EVENT_CONTEXT_RENEWED,
      EVENT_TRIGGER_BACK,
      EVENT_TRIGGER_ESCAPE
    };

    enum
    {
      ACTION_CAPTURE_BACK = 1,
      ACTION_RELEASE_BACK,
      ACTION_CAPTURE_ESCAPE,
      ACTION_RELEASE_ESCAPE,
    };

    #if defined(CHR_PLATFORM_DESKTOP)
      static void run(int width, int height, int aaSamples = 0, int depthBits = 0);
    #elif defined(CHR_PLATFORM_EMSCRIPTEN)
      static void run(int aaSamples = 0, int depthBits = 0);
    #endif

    virtual ~CrossSketch() = default;

    virtual bool init() { return true; }
    virtual void uninit() {}
    virtual void setup() {}
    virtual void shutdown() {}

    virtual void event(int eventId) {}
    virtual void handleMessage(int what, const std::string &body) {}

    virtual void resize() {}
    virtual void start(StartReason reason) {}
    virtual void stop(StopReason reason) {}

    virtual void update() {}
    virtual void draw() {}

    virtual void mouseMoved(float x, float y) {}
    virtual void mouseDragged(int button, float x, float y) {}
    virtual void mousePressed(int button, float x, float y) {}
    virtual void mouseReleased(int button, float x, float y) {}

    virtual void addTouch(int index, float x, float y) {}
    virtual void updateTouch(int index, float x, float y) {}
    virtual void removeTouch(int index, float x, float y) {}

    virtual void keyPressed(uint32_t codepoint) {}
    virtual void keyDown(int keyCode, int modifiers) {}
    virtual void keyUp(int keyCode, int modifiers) {}

    virtual void wheelUpdated(float offset) {}
    virtual void accelerated(AccelEvent event) {}

    double getElapsedSeconds();
    int getElapsedFrames();
    FrameClock::Ref clock() const;

    #if defined(CHR_PLATFORM_DESKTOP)
      void grabScreen(const fs::path &destinationDirectory);
    #endif

    void performSetup(const WindowInfo &windowInfo);
    void performResize(const glm::vec2 &size, float safeAreaInsetsTop = 0, float safeAreaInsetsBottom = 0);
    void performStart(StartReason reason);
    void performStop(StopReason reason);
    void performUpdate();

  protected:
    Timer timer;
    int frameCount = 0;
    int grabbedFrameCount = 0;
    bool forceResize = false;
    WindowInfo windowInfo;
    FrameClock::Ref _clock = FrameClock::create();

    #if defined(CHR_PLATFORM_DESKTOP)
      void performGrabScreen(const fs::path &filePath);
    #endif
  };
}
