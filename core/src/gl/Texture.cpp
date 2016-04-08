#include "gl/Texture.h"
#include "gl/Utils.h"

namespace chr
{
  namespace gl
  {
    Texture::Texture(const Request &request)
    {
      auto response = loadAndUploadTexture(request);

      textureId = response.textureId;
      format = response.format;
      width = response.width;
      height = response.height;
      maxU = response.maxU;
      maxV = response.maxV;
    }

    Texture::Texture(const MaskedRequest &request)
    {
      auto response = uploadMaskedTexture(request);

      textureId = response.textureId;
      format = response.format;
      width = response.width;
      height = response.height;
      maxU = response.maxU;
      maxV = response.maxV;
    }

    void Texture::bind() const
    {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textureId);
    }

    void Texture::unbind() const
    {
      glDisable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, 0);
    }
  }
}
