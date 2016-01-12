#pragma once

#include "gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class FogTextureAlphaShader : public ShaderProgram
    {
    public:
      static const char *vertexShaderSource;
      static const char *fragmentShaderSource;

      GLint positionLocation;
      GLint coordLocation;
      GLint colorLocation;
      GLint samplerLocation;
      GLint matrixLocation;

      GLint fogDensityLocation;
      GLint fogColorLocation;

      bool load();
      bool use();
    };

    extern FogTextureAlphaShader fogTextureAlphaShader;
  }
}
