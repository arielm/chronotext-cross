#pragma once

#include "gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class TextureAlphaShader : public ShaderProgram
    {
    public:
      bool load() override;
    };

    extern TextureAlphaShader textureAlphaShader;
  }
}
