#pragma once

#include "chr/image/Utils.h"
#include "chr/gl.h"

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
      /*
       * TODO: Handle mipmap and anisotropy
       */
      struct EmptyRequest
      {
        union
        {
          glm::ivec2 size;
          struct { int width, height; };
        };

        GLenum format = GL_RGBA;
        GLenum type = GL_UNSIGNED_BYTE;
        GLint minFilter = GL_LINEAR;
        GLint magFilter = GL_LINEAR;
        GLenum wrapS = GL_CLAMP_TO_EDGE;
        GLenum wrapT = GL_CLAMP_TO_EDGE;

        EmptyRequest() = default;

        EmptyRequest(int width, int height)
        :
        width(width),
        height(height)
        {}

        EmptyRequest(const glm::ivec2 &size)
        :
        size(size)
        {}

        EmptyRequest& setFormat(GLenum format)
        {
          this->format = format;
          return *this;
        }

        EmptyRequest& setType(GLenum type)
        {
          this->type = type;
          return *this;
        }

        EmptyRequest& setMinFilter(GLint min)
        {
          minFilter = min;
          return *this;
        }

        EmptyRequest& setMagFilter(GLint mag)
        {
          magFilter = mag;
          return *this;
        }

        EmptyRequest& setWrap(GLenum s, GLenum t)
        {
          wrapS = s;
          wrapT = t;
          return *this;
        }
      };

      struct ImageRequest
      {
        fs::path relativePath;
        int imageFlags = image::FLAGS_NONE;
        bool useMipmap = false;
        bool useAnisotropy = false;
        GLenum wrapS = GL_CLAMP_TO_EDGE;
        GLenum wrapT = GL_CLAMP_TO_EDGE;

        ImageRequest() = default;

        ImageRequest(const fs::path &relativePath)
        :
        relativePath(relativePath)
        {}

        ImageRequest& setFlags(int flags = image::FLAGS_NONE)
        {
          imageFlags = flags;
          return *this;
        }

        ImageRequest& setMipmap(bool mipmap)
        {
          useMipmap = mipmap;
          return *this;
        }

        ImageRequest& setAnisotropy(bool anisotropy)
        {
          useAnisotropy = anisotropy;
          return *this;
        }

        ImageRequest& setWrap(GLenum s, GLenum t)
        {
          wrapS = s;
          wrapT = t;
          return *this;
        }

        bool operator<(const ImageRequest &rhs) const
        {
          return
            std::tie(relativePath, imageFlags, useMipmap, useAnisotropy, wrapS, wrapT) <
            std::tie(rhs.relativePath, rhs.imageFlags, rhs.useMipmap, rhs.useAnisotropy, rhs.wrapS, rhs.wrapT);
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

        bool operator<(const MaskedRequest &rhs) const
        {
          return
            std::tie(imageRelativePath, maskRelativePath, imageFlags, maskFlags, useMipmap, useAnisotropy, wrapS, wrapT) <
            std::tie(rhs.imageRelativePath, rhs.maskRelativePath, rhs.imageFlags, rhs.maskFlags, rhs.useMipmap, rhs.useAnisotropy, rhs.wrapS, rhs.wrapT);
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
      Texture(const EmptyRequest &request);
      Texture(const ImageRequest &request);
      Texture(const MaskedRequest &request);
      Texture(const Response &response);
      Texture(const Texture &other);
      Texture& operator=(const Texture &other);

      ~Texture();

      bool operator<(const Texture &rhs) const
      {
        return id < rhs.id;
      }

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
        REQUEST_EMPTY,
        REQUEST_IMAGE,
        REQUEST_MASKED
      };

      RequestType requestType;
      ImageRequest imageRequest;
      MaskedRequest maskedRequest;

      static int usageCounter;

      bool load();
    };
  }
}
