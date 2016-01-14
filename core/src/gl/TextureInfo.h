#pragma once

#include "gl/Utils.h"

namespace chr
{
  namespace gl
  {
    struct TextureInfo
    {
      GLuint id;
      GLenum format;
      int width;
      int height;
    };
  }
}
