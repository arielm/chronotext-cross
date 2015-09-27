#pragma once

#include "gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class TextureAlphaShader : public ShaderProgram
    {
    public:
      static const char *vertexShaderSource;
      static const char *fragmentShaderSource;

      GLint positionLocation;
      GLint coordLocation;
      GLint colorLocation;
      GLint samplerLocation;
      GLint matrixLocation;

      bool load();
      bool use();
    };

    static TextureAlphaShader textureAlphaShader;
  }
}
