#pragma once

#include "gl/gl.h"

namespace chr
{
  namespace gl
  {
    struct Texture
    {
      GLuint id;
      GLenum format;
      int width;
      int height;
      float maxU;
      float maxV;
    };
  }
}
