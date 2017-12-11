#include "chr/gl/Texture.h"
#include "chr/gl/Utils.h"

namespace chr
{
  namespace gl
  {
    int Texture::usageCounter = 0;

    Texture::Texture()
    {}

    Texture::Texture(const Format &format)
    :
    id(usageCounter++),
    element(new texture::Element()),
    format(format.format),
    size(format.size),
    innerSize(format.size),
    coords1(1),
    coords2(1)
    {
      glGenTextures(1, &textureId);

      glBindTexture(GL_TEXTURE_2D, textureId);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, format.minFilter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, format.magFilter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format.wrapS);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format.wrapT);
      glTexImage2D(GL_TEXTURE_2D, 0, format.format, format.width, format.height, 0, format.format, format.type, nullptr);
      glBindTexture(GL_TEXTURE_2D, 0);

      element->textureId = textureId;
      element->useCount++;
    }

    Texture::Texture(const ImageRequest &request)
    :
    Texture(loadAndUploadTexture(request))
    {
      requestType = REQUEST_IMAGE,
      imageRequest = request;
    }

    Texture::Texture(const MaskedRequest &request)
    :
    Texture(loadAndUploadMaskedTexture(request))
    {
      requestType = REQUEST_MASKED,
      maskedRequest = request;
    }

    Texture::Texture(const Response &response)
    :
    id(usageCounter++),
    element(new texture::Element()),
    format(response.format),
    size(response.size),
    innerSize(response.innerSize),
    coords1(response.coords1),
    coords2(response.coords2)
    {
      element->textureId = response.textureId;
      element->useCount++;
    }

    Texture::Texture(const Texture &other)
    :
    id(other.id),
    element(other.element),
    format(other.format),
    textureId(other.textureId),
    size(other.size),
    innerSize(other.innerSize),
    coords1(other.coords1),
    coords2(other.coords2),
    requestType(other.requestType),
    imageRequest(other.imageRequest),
    maskedRequest(other.maskedRequest)
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
        textureId = other.textureId;
        size = other.size;
        innerSize = other.innerSize;
        coords1 = other.coords1;
        coords2 = other.coords2;
        requestType = other.requestType;
        imageRequest = other.imageRequest;
        maskedRequest = other.maskedRequest;

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
        glBindTexture(GL_TEXTURE_2D, element->textureId);

        return true;
      }

      return false;
    }

    void Texture::unbind()
    {
      if (element && element->textureId)
      {
        glBindTexture(GL_TEXTURE_2D, 0);
      }
    }

    bool Texture::load()
    {
      if (element && !element->textureId)
      {
        if (requestType == REQUEST_IMAGE)
        {
          const auto response = loadAndUploadTexture(imageRequest);
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
