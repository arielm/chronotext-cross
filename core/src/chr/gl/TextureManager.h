#pragma once

#include "chr/gl/Texture.h"

namespace chr
{
  namespace gl
  {
    class TextureManager
    {
    public:
      Texture get(const Texture::Request &request);
      Texture get(const Texture::MaskedRequest &request);

    protected:
      std::map<Texture::Request, Texture> textures;
      std::map<Texture::MaskedRequest, Texture> maskedTextures;
    };
  }
}
