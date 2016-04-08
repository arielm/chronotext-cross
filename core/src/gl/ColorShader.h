#pragma once

#include "gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class ColorShader : public ShaderProgram
    {
    public:
      bool load() override;
    };

    extern ColorShader colorShader;
  }
}
