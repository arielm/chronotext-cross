#pragma once

#include "gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class FogColorShader : public ShaderProgram
    {
    public:
      static const char *vertexShaderSource;
      static const char *fragmentShaderSource;

      GLuint fogDensityLocation;
      GLuint fogColorLocation;

      bool load();
    };

    extern FogColorShader fogColorShader;
  }
}
