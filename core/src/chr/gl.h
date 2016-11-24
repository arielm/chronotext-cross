#pragma once

#include "glm.h"
#include "chr/Platform.h"

#if defined(CHR_PLATFORM_GLFW)
  #include <glad/glad.h>
#elif defined(CHR_PLATFORM_WIN)
  // TODO
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
  #include <html5.h>
#elif defined(CHR_PLATFORM_IOS)
  #include <OpenGLES/ES2/gl.h>
  #include <OpenGLES/ES2/glext.h>
#elif defined(CHR_PLATFORM_ANDROID)
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
#endif

#if defined (CHR_PLATFORM_GLFW)
  #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
  #define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

  #define GL_GENERATE_MIPMAP 0x8191
  #define GL_GENERATE_MIPMAP_HINT 0x8192
#endif
