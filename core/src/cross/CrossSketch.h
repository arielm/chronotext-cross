#pragma once

#include "Log.h"
#include "Platform.h"
#include "MemoryBuffer.h"
#include "cross/WindowInfo.h"

#if defined(CHR_PLATFORM_DESKTOP)
  #include <glad/glad.h>
  #include <GLFW/glfw3.h>
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include <emscripten.h>
  #include <emscripten/html5.h>
  #include <GLES2/gl2.h>
#elif defined(CHR_PLATFORM_IOS)
  #include <OpenGLES/ES2/gl.h>
  #include <OpenGLES/ES2/glext.h>
#elif defined(CHR_PLATFORM_ANDROID)
  #include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#endif

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>

// ---

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

static constexpr float D2R (M_PI / 180.0);
static constexpr float R2D (180.0 / M_PI);

// ---

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

    static GLuint makeShader(GLenum type, const char *text);
    static GLuint makeShaderProgram(const char *vs_text, const char *fs_text);
    static GLuint loadTexture(const fs::path &relativePath);

    static void run(int width, int height, int aaSamples = 0);
    static double getTime();

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

    void performSetup(const WindowInfo &windowInfo);
    void performResize(const glm::vec2 &size);
    void performStart(StartReason reason);
    void performStop(StopReason reason);
    void performUpdate();

  protected:
    int frameCount = 0;
    bool forceResize = false;
    WindowInfo windowInfo;
  };
}
