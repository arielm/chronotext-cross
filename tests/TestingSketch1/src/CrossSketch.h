#pragma once

#include "Log.h"
#include "Platform.h"

#if defined(CHR_PLATFORM_DESKTOP)
  #include <glad/glad.h>
  #include <GLFW/glfw3.h>
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include <emscripten.h>
  #include <emscripten/html5.h>
  #include <GLES2/gl2.h>
#endif

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace chr
{
  class CrossSketch
  {
  public:
    static GLuint makeShader(GLenum type, const char *text);
    static GLuint makeShaderProgram(const char *vs_text, const char *fs_text);

  #if defined(CHR_PLATFORM_DESKTOP)
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  #elif defined(CHR_PLATFORM_EMSCRIPTEN)
    static void performDraw(void *data);
  #endif

    void init(int width, int height);
    double getTime();

    virtual void setup() {}
    virtual void shutdown() {}
    virtual void draw() {}

  protected:
    bool initialized = false;

  #if defined(CHR_PLATFORM_DESKTOP)
    GLFWwindow* window = nullptr;
  #endif
  };
}
