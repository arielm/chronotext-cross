#pragma once

#include "Platform.h"

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#if defined(CHR_PLATFORM_DESKTOP)
  #include <glad/glad.h>
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
#elif defined(CHR_PLATFORM_IOS)
  #include <OpenGLES/ES2/gl.h>
  #include <OpenGLES/ES2/glext.h>
#elif defined(CHR_PLATFORM_ANDROID)
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
#endif

#if defined (CHR_PLATFORM_DESKTOP)
  #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
  #define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

  #define GL_GENERATE_MIPMAP 0x8191
  #define GL_GENERATE_MIPMAP_HINT 0x8192
#endif

namespace chr
{
  namespace gl
  {
    GLuint loadTexture(const fs::path &relativePath, bool forceAlpha = false);
    void uploadTextureData(GLenum format, GLsizei width, GLsizei height, const GLvoid *data);

    const glm::mat4 getPerspectiveMatrix(float fovy, float zNear, float zFar, float width, float height, float panX, float panY, float zoom);
  }
}
