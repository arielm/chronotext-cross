#include "chr/gl/Texture.h"
#include "chr/gl/Utils.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    int Texture::usageCounter = 0;

    Texture::Texture()
    {}

    Texture::Texture(GLuint textureId, int width, int height, GLenum format, GLenum target)
    :
    id(usageCounter++),
    element(new texture::Element()),
    width(width),
    height(height),
    innerWidth(width),
    innerHeight(height),
    coords1(0),
    coords2(1),
    target(target)
    {
      element->textureId = textureId;
      element->format = format;
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
    size(response.size),
    innerSize(response.innerSize),
    coords1(response.coords1),
    coords2(response.coords2),
    target(GL_TEXTURE_2D)
    {
      element->textureId = response.textureId;
      element->format = response.format;
      element->useCount++;
    }

    Texture::Texture(const Texture &other)
    :
    id(other.id),
    element(other.element),
    size(other.size),
    innerSize(other.innerSize),
    coords1(other.coords1),
    coords2(other.coords2),
    target(other.target)
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
        size = other.size;
        innerSize = other.innerSize;
        coords1 = other.coords1;
        coords2 = other.coords2;
        target = other.target;

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
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(target, element->textureId);
        return true;
      }

      return false;
    }

    void Texture::unbind()
    {
      if (element && element->textureId)
      {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(target, 0);
      }
    }

    void Texture::unload()
    {
      if (element && element->textureId)
      {
        glDeleteTextures(1, &element->textureId);
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
