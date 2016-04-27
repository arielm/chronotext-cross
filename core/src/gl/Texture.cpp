#include "gl/Texture.h"
#include "gl/Utils.h"

namespace chr
{
  namespace gl
  {
    int Texture ::usageCounter = 0;

    Texture::Texture(const Response &response)
    :
    id(usageCounter++),
    element(new texture::Element()),
    format(response.format),
    width(response.width),
    height(response.height),
    innerWidth(response.innerWidth),
    innerHeight(response.innerHeight),
    u1(response.u1),
    v1(response.v1),
    u2(response.u2),
    v2(response.v2)
    {
      element->textureId = response.textureId;
      element->useCount++;
    }

    Texture::Texture(const Request &request)
    :
    Texture(loadAndUploadTexture(request))
    {
      requestType = REQUEST_REGULAR,
      this->request = request;
    }

    Texture::Texture(const MaskedRequest &request)
    :
    Texture(loadAndUploadMaskedTexture(request))
    {
      requestType = REQUEST_MASKED,
      maskedRequest = request;
    }

    Texture::Texture(const Texture &other)
    :
    id(other.id),
    element(other.element),
    format(other.format),
    width(other.width),
    height(other.height),
    innerWidth(other.innerWidth),
    innerHeight(other.innerHeight),
    u1(other.u1),
    v1(other.v1),
    u2(other.u2),
    v2(other.v2)
    {
      if (element)
      {
        element->useCount++;
      }
    }

    Texture& Texture::operator=(const Texture &other)
    {
      if (this != &other)
      {
        id = other.id;
        element = other.element;
        format = other.format;
        width = other.width;
        height = other.height;
        innerWidth = other.innerWidth;
        innerHeight = other.innerHeight;
        u1 = other.u1;
        v1 = other.v1;
        u2 = other.u2;
        v2 = other.v2;

        if (element)
        {
          element->useCount++;
        }
      }

      return *this;
    }

    Texture::~Texture()
    {
      if (element)
      {
        element->useCount--;

        if (element->useCount == 0)
        {
          unload();
          delete element;
        }
      }
    }

    bool Texture::bind()
    {
      if (load())
      {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, element->textureId);

        return true;
      }

      return false;
    }

    void Texture::unbind()
    {
      if (element && element->textureId)
      {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
      }
    }

    bool Texture::load()
    {
      if (element && !element->textureId)
      {
        if (requestType == REQUEST_REGULAR)
        {
          const auto response = loadAndUploadTexture(request);
          element->textureId = response.textureId;

          return true;
        }
        else if (requestType == REQUEST_MASKED)
        {
          const auto response = loadAndUploadMaskedTexture(maskedRequest);
          element->textureId = response.textureId;

          return true;
        }
      }

      return (element && element->textureId);
    }

    void Texture::unload()
    {
      if (element && element->textureId)
      {
        glDeleteTextures(1, &element->textureId);
        element->textureId = 0;
      }
    }
  }
}
