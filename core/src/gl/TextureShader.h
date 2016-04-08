#pragma once

#include "gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class TextureShader : public ShaderProgram
    {
    public:
      bool load() override;
    };

    extern TextureShader textureShader;
  }
}
