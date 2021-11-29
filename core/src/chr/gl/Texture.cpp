#include "chr/gl/Texture.h"
#include "chr/gl/Utils.h"
#include "chr/Log.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    int Texture::usageCounter = 0;

    Texture::Texture()
    {}

    Texture::Texture(GLuint textureId, int width, int height, GLenum format)
    :
    id(usageCounter++),
    element(new texture::Element()),
    textureId(textureId),
    width(width),
    height(height),
    format(format),
    innerWidth(width),
    innerHeight(height),
    coords1(0),
    coords2(1)
    {
      element->textureId = textureId;
      element->useCount++;
    }

    Texture::Texture(const Params &params)
    :
    id(usageCounter++),
    element(new texture::Element()),
    format(params.format),
    size(params.size),
    innerSize(params.size),
    coords1(0),
    coords2(1)
    {
      glGenTextures(1, &textureId);

      glBindTexture(GL_TEXTURE_2D, textureId);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
      glTexImage2D(GL_TEXTURE_2D, 0, params.format, params.width, params.height, 0, params.format, params.type, nullptr);
      glBindTexture(GL_TEXTURE_2D, 0);

      element->textureId = textureId;
      element->useCount++;
    }

    Texture::Texture(const ImageRequest &request)
    :
    Texture(loadAndUploadTexture(request))
    {}

    Texture::Texture(const MaskedRequest &request)
    :
    Texture(loadAndUploadMaskedTexture(request))
    {}

    Texture::Texture(const Response &response)
    :
    id(usageCounter++),
    element(new texture::Element()),
    format(response.format),
    textureId(response.textureId),
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
    coords2(other.coords2)
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

        if (element)
        {
          element->useCount++;
        }
      }

      return *this;
    }

    Texture::~Texture()
    {
      purge();
    }

    bool Texture::bind()
    {
      if (element && element->textureId)
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

    void Texture::unload()
    {
      if (element && element->textureId)
      {
        glDeleteTextures(1, &element->textureId);
        LOGD << "TEXTURE " << element->textureId << " UNLOADED" << endl;
        element->textureId = 0;
      }
    }

    void Texture::purge()
    {
      if (element)
      {
        element->useCount--;

        if (element->useCount == 0)
        {
          unload();
          delete element;
          element = nullptr;
        }
      }
    }
  }
}
