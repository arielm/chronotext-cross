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
        int useCount;
        GLuint textureId;
      };
    }

    class Texture
    {
    public:
      struct Request
      {
        fs::path relativePath;
        int imageFlags;
        bool useMipmap;
        GLenum wrapS;
        GLenum wrapT;

        Request() = default;

        Request(const fs::path &relativePath)
        :
        relativePath(relativePath),
        imageFlags(image::FLAGS_NONE),
        useMipmap(false),
        wrapS(GL_CLAMP_TO_EDGE),
        wrapT(GL_CLAMP_TO_EDGE)
        {}

        Request& setFlags(int flags = image::FLAGS_NONE)
        {
          imageFlags = flags;
          return *this;
        }

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
        fs::path imageRelativePath;
        fs::path maskRelativePath;
        int imageFlags;
        int maskFlags;
        bool useMipmap;
        GLenum wrapS;
        GLenum wrapT;

        MaskedRequest() = default;

        MaskedRequest(const fs::path &imageRelativePath, const fs::path &maskRelativePath)
        :
        imageRelativePath(imageRelativePath),
        maskRelativePath(maskRelativePath),
        imageFlags(image::FLAGS_NONE),
        maskFlags(image::FLAGS_NONE),
        useMipmap(false),
        wrapS(GL_CLAMP_TO_EDGE),
        wrapT(GL_CLAMP_TO_EDGE)
        {}

        MaskedRequest& setFlags(int imageFlags = image::FLAGS_NONE, int maskFlags = image::FLAGS_NONE)
        {
          this->imageFlags = imageFlags;
          this->maskFlags = maskFlags;
          return *this;
        }

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

      // ---

      Texture() = default;
      Texture(const Response &response);
      Texture(const Request &request);
      Texture(const MaskedRequest &request);
      Texture(const Texture &other);
      Texture& operator=(const Texture &other);

      ~Texture();

      int id;
      texture::Element *element;

      GLenum format;
      int width;
      int height;
      float maxU;
      float maxV;

      void bind();
      void unbind();

      bool reload();
      bool unload();

    protected:
      enum RequestType
      {
        REQUEST_REGULAR = 1,
        REQUEST_MASKED = 2
      };

      RequestType requestType;
      Request request;
      MaskedRequest maskedRequest;

      static int usageCounter;
    };
  }
}
