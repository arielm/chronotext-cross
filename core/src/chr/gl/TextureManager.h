#pragma once

#include "chr/gl/Texture.h"

namespace chr
{
  namespace gl
  {
    class TextureManager
    {
    public:
      Texture get(const Texture::ImageRequest &request);
      Texture get(const Texture::MaskedRequest &request);

    protected:
      std::map<Texture::ImageRequest, Texture> textures;
      std::map<Texture::MaskedRequest, Texture> maskedTextures;
    };
  }
}
