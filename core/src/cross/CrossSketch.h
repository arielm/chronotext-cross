#pragma once

#include "Timer.h"

#include "cross/AccelEvent.h"
#include "cross/WindowInfo.h"
#include "system/DisplayHelper.h"
#include "time/FrameClock.h"
#include "utils/Utils.h"
#include "math/Utils.h"
#include "gl/Utils.h"

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>

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

    static void run(int width, int height, int aaSamples = 0);

    virtual bool init() { return true; }
    virtual void uninit() {}
    virtual void setup() {}
    virtual void shutdown() {}

    virtual void event(int eventId) {}

    virtual void resize() {}
    virtual void start(StartReason reason) {}
    virtual void stop(StopReason reason) {}

    virtual void update() {}
    virtual void draw() {}

    virtual void addTouch(int index, float x, float y) {}
    virtual void updateTouch(int index, float x, float y) {}
    virtual void removeTouch(int index, float x, float y) {}

    virtual void accelerated(AccelEvent event) {}

    const WindowInfo& getWindowInfo() const;
    double getElapsedSeconds();
    int getElapsedFrames();
    FrameClock::Ref clock() const;

    void performSetup(const WindowInfo &windowInfo);
    void performResize(const glm::vec2 &size);
    void performStart(StartReason reason);
    void performStop(StopReason reason);
    void performUpdate();

  protected:
    Timer timer;
    int frameCount = 0;
    bool forceResize = false;
    WindowInfo windowInfo;
    FrameClock::Ref _clock = FrameClock::create();
  };
}
