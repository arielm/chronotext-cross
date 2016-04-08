#pragma once

#include "gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class ColorShader : public ShaderProgram
    {
    public:
      ColorShader();
      bool load() override;
    };

    extern ColorShader colorShader;
  }
}
