#pragma once

#include "gl/Utils.h"

namespace chr
{
  namespace gl
  {
    struct TextureHandle
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
