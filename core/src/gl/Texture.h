#pragma once

#include "image/Utils.h"
#include "gl.h"

namespace chr
{
  namespace gl
  {
    namespace texture
    {
      struct Element
      {
        int useCount = 0;
        GLuint textureId;
      };
    }

    class Texture
    {
    public:
      struct Request
      {
        fs::path relativePath;
        int imageFlags = image::FLAGS_NONE;
        bool useMipmap = false;
        bool useAnisotropy = false;
        GLenum wrapS = GL_CLAMP_TO_EDGE;
        GLenum wrapT = GL_CLAMP_TO_EDGE;

        Request() = default;

        Request(const fs::path &relativePath)
        :
        relativePath(relativePath)
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

        Request& setAnisotropy(bool anisotropy)
        {
          useAnisotropy = anisotropy;
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
        int imageFlags = image::FLAGS_NONE;
        int maskFlags = image::FLAGS_NONE;
        bool useMipmap = false;
        bool useAnisotropy = false;
        GLenum wrapS = GL_CLAMP_TO_EDGE;
        GLenum wrapT = GL_CLAMP_TO_EDGE;

        MaskedRequest() = default;

        MaskedRequest(const fs::path &imageRelativePath, const fs::path &maskRelativePath)
        :
        imageRelativePath(imageRelativePath),
        maskRelativePath(maskRelativePath)
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

        MaskedRequest& setAnisotropy(bool anisotropy)
        {
          useAnisotropy = anisotropy;
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

        union
        {
          glm::vec2 size;
          struct { float width, height; };
        };

        union
        {
          glm::vec2 innerSize;
          struct { float innerWidth, innerHeight; };
        };

        union
        {
          glm::vec2 coords1;
          struct { float u1, v1; };
        };

        union
        {
          glm::vec2 coords2;
          struct { float u2, v2; };
        };

        Response()
        {}
      };

      // ---

      Texture();
      Texture(const Response &response);
      Texture(const Request &request);
      Texture(const MaskedRequest &request);
      Texture(const Texture &other);
      Texture& operator=(const Texture &other);

      ~Texture();

      int id = -1;
      texture::Element *element = nullptr;

      GLenum format;
      GLuint textureId;

      union
      {
        glm::vec2 size;
        struct { float width, height; };
      };

      union
      {
        glm::vec2 innerSize;
        struct { float innerWidth, innerHeight; };
      };

      union
      {
        glm::vec2 coords1;
        struct { float u1, v1; };
      };

      union
      {
        glm::vec2 coords2;
        struct { float u2, v2; };
      };

      bool bind();
      void unbind();
      void unload();

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

      bool load();
    };
  }
}
