#pragma once

#include "glm.h"
#include "chr/Platform.h"

#if defined(CHR_PLATFORM_OSX)
  #include <OpenGL/gl.h>
  #include <OpenGL/glext.h>
#elif defined(CHR_PLATFORM_WIN)
  #include "glload/gl_2_0.h"
  #include "glload/gl_load.h"
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #define GL_GLEXT_PROTOTYPES
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
  #include <emscripten/html5.h>
#elif defined(CHR_PLATFORM_IOS)
  #include <OpenGLES/ES2/gl.h>
  #include <OpenGLES/ES2/glext.h>
#elif defined(CHR_PLATFORM_ANDROID)
  #include <GLES3/gl3.h>
  #include <GLES3/gl3ext.h>
#elif defined(CHR_PLATFORM_RPI) || defined(CHR_PLATFORM_RPI64) || defined(CHR_PLATFORM_LINUX)
  #include "glad/gl.h"
#endif

#if defined (CHR_PLATFORM_DESKTOP)
  #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
  #define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif

#if defined(CHR_PLATFORM_EMSCRIPTEN)
  #define glVertexAttribDivisor glVertexAttribDivisorANGLE
  #define glDrawArraysInstanced glDrawArraysInstancedANGLE
  #define glDrawElementsInstanced glDrawElementsInstancedANGLE
#elif defined(CHR_PLATFORM_IOS)
  #define glVertexAttribDivisor glVertexAttribDivisorEXT
  #define glDrawArraysInstanced glDrawArraysInstancedEXT
  #define glDrawElementsInstanced glDrawElementsInstancedEXT
#elif defined(CHR_PLATFORM_OSX) || defined(CHR_PLATFORM_WIN)
  #define glVertexAttribDivisor glVertexAttribDivisorARB
  #define glDrawArraysInstanced glDrawArraysInstancedARB
  #define glDrawElementsInstanced glDrawElementsInstancedARB
#endif
