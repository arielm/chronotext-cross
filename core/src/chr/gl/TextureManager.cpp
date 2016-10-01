#include "chr/gl/TextureManager.h"

namespace chr
{
  namespace gl
  {
    Texture TextureManager::get(const Texture::Request &request)
    {
      auto found = textures.find(request);

      if (found == textures.end())
      {
        Texture texture = Texture(request);
        textures.emplace(request, texture);

        return texture;
      }
      else
      {
        return found->second;
      }
    }

    Texture TextureManager::get(const Texture::MaskedRequest &request)
    {
      auto found = maskedTextures.find(request);

      if (found == maskedTextures.end())
      {
        Texture texture = Texture(request);
        maskedTextures.emplace(request, texture);

        return texture;
      }
      else
      {
        return found->second;
      }
    }
  }
}
