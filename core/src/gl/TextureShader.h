#pragma once

#include "gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class TextureShader : public ShaderProgram
    {
    public:
      static const char *vertexShaderSource;
      static const char *fragmentShaderSource;

      bool load();
    };

    extern TextureShader textureShader;
  }
}
