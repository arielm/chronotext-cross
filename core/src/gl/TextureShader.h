#pragma once

#include "gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class TextureShader : public ShaderProgram
    {
    public:
      TextureShader();
      bool load() override;
    };

    extern TextureShader textureShader;
  }
}
