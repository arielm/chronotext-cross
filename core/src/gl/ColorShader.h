#pragma once

#include "gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class ColorShader : public ShaderProgram
    {
    public:
      static const char *vertexShaderSource;
      static const char *fragmentShaderSource;

      bool load();
      bool use() override;
    };

    extern ColorShader colorShader;
  }
}
