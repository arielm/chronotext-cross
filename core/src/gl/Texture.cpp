#include "gl/Texture.h"
#include "gl/Utils.h"

namespace chr
{
  namespace gl
  {
    Texture::Texture(const Response &response)
    :
    textureId(response.textureId),
    format(response.format),
    width(response.width),
    height(response.height),
    maxU(response.maxU),
    maxV(response.maxV)
    {}

    Texture::Texture(const Request &request)
    :
    Texture(loadAndUploadTexture(request))
    {}

    Texture::Texture(const MaskedRequest &request)
    :
    Texture(uploadMaskedTexture(request))
    {}

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
