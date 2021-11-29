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
      struct Params
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

        Params() = default;

        Params(int width, int height)
        :
        width(width),
        height(height)
        {}

        Params(const glm::ivec2 &size)
        :
        size(size)
        {}

        Params& setFormat(GLenum format)
        {
          this->format = format;
          return *this;
        }

        Params& setType(GLenum type)
        {
          this->type = type;
          return *this;
        }

        Params& setMinFilter(GLint min)
        {
          minFilter = min;
          return *this;
        }

        Params& setMagFilter(GLint mag)
        {
          magFilter = mag;
          return *this;
        }

        Params& setWrap(GLenum s, GLenum t)
        {
          wrapS = s;
          wrapT = t;
          return *this;
        }
      };

      struct ImageRequest
      {
        InputSource source;
        int imageFlags = image::FLAGS_NONE;
        bool useMipmap = false;
        bool useAnisotropy = false;
        GLenum wrapS = GL_CLAMP_TO_EDGE;
        GLenum wrapT = GL_CLAMP_TO_EDGE;
        GLint minFilter = GL_LINEAR;
        GLint magFilter = GL_LINEAR;

        ImageRequest() = default;

        ImageRequest(const InputSource &source)
        :
        source(source)
        {}

        ImageRequest(const fs::path &resourcePath)
        :
        ImageRequest(InputSource::resource(resourcePath))
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

        ImageRequest& setFilters(GLint min, GLint mag)
        {
          minFilter = min;
          magFilter = mag;
          return *this;
        }

        bool operator<(const ImageRequest &rhs) const
        {
          return
            std::tie(source.getUri(), imageFlags, useMipmap, useAnisotropy, wrapS, wrapT, minFilter, magFilter) <
            std::tie(rhs.source.getUri(), rhs.imageFlags, rhs.useMipmap, rhs.useAnisotropy, rhs.wrapS, rhs.wrapT, rhs.minFilter, rhs.magFilter);
        }
      };

      struct MaskedRequest
      {
        InputSource imageSource;
        InputSource maskSource;
        int imageFlags = image::FLAGS_NONE;
        int maskFlags = image::FLAGS_NONE;
        bool useMipmap = false;
        bool useAnisotropy = false;
        GLenum wrapS = GL_CLAMP_TO_EDGE;
        GLenum wrapT = GL_CLAMP_TO_EDGE;
        GLint minFilter = GL_LINEAR;
        GLint magFilter = GL_LINEAR;

        MaskedRequest() = default;

        MaskedRequest(const InputSource &imageSource, const InputSource &maskSource)
        :
        imageSource(imageSource),
        maskSource(maskSource)
        {}

        MaskedRequest(const fs::path &imageResourcePath, const fs::path &maskResourcePath)
        :
        MaskedRequest(InputSource::resource(imageResourcePath), InputSource::resource(maskResourcePath))
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

        MaskedRequest& setFilters(GLint min, GLint mag)
        {
          minFilter = min;
          magFilter = mag;
          return *this;
        }

        bool operator<(const MaskedRequest &rhs) const
        {
          return
            std::tie(imageSource.getUri(), maskSource.getUri(), imageFlags, maskFlags, useMipmap, useAnisotropy, wrapS, wrapT, minFilter, magFilter) <
            std::tie(rhs.imageSource.getUri(), rhs.maskSource.getUri(), rhs.imageFlags, rhs.maskFlags, rhs.useMipmap, rhs.useAnisotropy, rhs.wrapS, rhs.wrapT, rhs.minFilter, rhs.magFilter);
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
      Texture(GLuint textureId, int width, int height, GLenum format = GL_RGBA);
      Texture(const Params &params);
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
      void purge();

    protected:
      static int usageCounter;
    };
  }
}
