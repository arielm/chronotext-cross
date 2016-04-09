#pragma once

#include "image/Utils.h"
#include "gl/gl.h"

namespace chr
{
  namespace gl
  {
    namespace texture
    {
      struct Element
      {
        GLuint textureId;
        GLenum format;
        int width;
        int height;
        float maxU;
        float maxV;
      };
    }

    struct Texture
    {
      struct Request
      {
        fs::path relativePath;
        int imageFlags;
        bool useMipmap;
        GLenum wrapS;
        GLenum wrapT;

        Request(const fs::path &relativePath, int imageFlags = image::FLAGS_NONE)
        :
        relativePath(relativePath),
        imageFlags(imageFlags),
        useMipmap(false),
        wrapS(GL_CLAMP_TO_EDGE),
        wrapT(GL_CLAMP_TO_EDGE)
        {}

        Request& setMipmap(bool mipmap)
        {
          useMipmap = mipmap;
          return *this;
        }

        Request& setWrap(GLenum s, GLenum t)
        {
          wrapS = s;
          wrapT = t;
          return *this;
        }
      };

      struct MaskedRequest
      {
        const image::ImageBuffer *image;
        const image::ImageBuffer *mask;
        bool useMipmap;
        GLenum wrapS;
        GLenum wrapT;

        MaskedRequest(const image::ImageBuffer *image, const image::ImageBuffer *mask)
        :
        image(image),
        mask(mask),
        useMipmap(false),
        wrapS(GL_CLAMP_TO_EDGE),
        wrapT(GL_CLAMP_TO_EDGE)
        {}

        MaskedRequest& setMipmap(bool mipmap)
        {
          useMipmap = mipmap;
          return *this;
        }

        MaskedRequest& setWrap(GLenum s, GLenum t)
        {
          wrapS = s;
          wrapT = t;
          return *this;
        }
      };

      struct Response
      {
        GLuint textureId;
        GLenum format;
        int width;
        int height;
        float maxU;
        float maxV;
      };

      GLuint textureId;
      GLenum format;
      int width;
      int height;
      float maxU;
      float maxV;

      Texture() = default;
      Texture(const Response &response);
      Texture(const Request &request);
      Texture(const MaskedRequest &request);

      void bind();
      void unbind();

    protected:
      static int usageCounter;
    };
  }
}
