#pragma once

#include "Platform.h"

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

namespace chr
{
  namespace gl
  {
    GLuint makeShader(GLenum type, const char *source);
    GLuint makeShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource);

    GLuint loadTexture(const fs::path &relativePath, bool forceAlpha = false);
    void uploadTextureData(GLenum format, GLsizei width, GLsizei height, const GLvoid *data);
  }
}
