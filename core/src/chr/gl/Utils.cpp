#include "chr/gl/Utils.h"

#if defined(CHR_PLATFORM_RPI) || defined(CHR_PLATFORM_RPI64)
  #include <GL/glu.h>
#endif

using namespace std;

namespace chr
{
  namespace gl
  {
    Texture::Response loadAndUploadTexture(const Texture::ImageRequest &request)
    {
      return uploadTexture(image::loadImage(request.source, request.imageFlags), request.useMipmap, request.useAnisotropy, request.wrapS, request.wrapT, request.minFilter, request.magFilter);
    }

    Texture::Response uploadTexture(const image::ImageBuffer &image, bool useMipmap, bool useAnisotropy, GLenum wrapS, GLenum wrapT, GLint minFilter, GLint magFilter)
    {
      Texture::Response response;

      if (image.isValid())
      {
        switch (image.components)
        {
          case 1:
            response.format = GL_ALPHA;
            break;

          case 3:
            response.format = GL_RGB;
            break;

          case 4:
            response.format = GL_RGBA;
            break;
        }

        if (response.format)
        {
          GLuint textureId = 0u;
          glGenTextures(1, &textureId);

          response.textureId = textureId;
          response.width = image.width;
          response.height = image.height;
          response.innerWidth = image.innerWidth;
          response.innerHeight = image.innerHeight;
          response.u1 = 0;
          response.v1 = 0;
          response.u2 = image.innerWidth / (float) image.width;
          response.v2 = image.innerHeight / (float) image.height;

          glBindTexture(GL_TEXTURE_2D, textureId);
          uploadTexture(response.format, response.width, response.height, image.buffer.get(), useMipmap, useAnisotropy, wrapS, wrapT, minFilter, magFilter);
        }
      }

      return response;
    }

    Texture::Response loadAndUploadMaskedTexture(const Texture::MaskedRequest &request)
    {
      const auto image = image::loadImage(request.imageSource, request.imageFlags);
      const auto mask = image::loadImage(request.maskSource, request.maskFlags);

      return uploadMaskedTexture(image, mask, request.useMipmap, request.useAnisotropy, request.wrapS, request.wrapT, request.minFilter, request.magFilter);
    }

    Texture::Response uploadMaskedTexture(const image::ImageBuffer &image, const image::ImageBuffer &mask, bool useMipmap, bool useAnisotropy, GLenum wrapS, GLenum wrapT, GLint minFilter, GLint magFilter)
    {
      Texture::Response response;

      if ((image.components == 4) && (mask.components == 1))
      {
        if ((mask.width >= image.innerWidth) && (mask.height >= image.innerHeight))
        {
          auto input = mask.buffer.get();
          auto output = image.buffer.get();

          int width = image.innerWidth;
          int height = image.innerHeight;

          for (int iy = 0; iy < height; iy++)
          {
            uint8_t *inputLine = input + (mask.width) * iy;
            uint8_t *outputLine = output + (4 * image.width) * iy;

            for (int ix = 0; ix < width; ix++)
            {
              outputLine += 3;
              *outputLine++ = *inputLine++;
            }
          }
        }

        GLuint textureId = 0u;
        glGenTextures(1, &textureId);

        response.textureId = textureId;
        response.format = GL_RGBA;
        response.width = image.width;
        response.height = image.height;
        response.innerWidth = image.innerWidth;
        response.innerHeight = image.innerHeight;
        response.u1 = 0;
        response.v1 = 0;
        response.u2 = image.innerWidth / (float) image.width;
        response.v2 = image.innerHeight / (float) image.height;

        glBindTexture(GL_TEXTURE_2D, textureId);
        uploadTexture(response.format, response.width, response.height, image.buffer.get(), useMipmap, useAnisotropy, wrapS, wrapT, minFilter, magFilter);
      }

      return  response;
    }

    void uploadTexture(GLenum format, GLsizei width, GLsizei height, const GLvoid *data, bool useMipmap, bool useAnisotropy, GLenum wrapS, GLenum wrapT, GLint minFilter, GLint magFilter)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

      if (useAnisotropy)
      {
        float anisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
      }

      if (useMipmap)
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      }
      else
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
      }

      #if defined(CHR_PLATFORM_RPI) || defined(CHR_PLATFORM_RPI64)
        if (useMipmap)
        {
          gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data);
        }
        else
        {
          glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        }
      #else
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        if (useMipmap)
        {
          glGenerateMipmap(GL_TEXTURE_2D);
        }
      #endif
    }

    const glm::mat4 getPerspectiveMatrix(float fovY, float nearZ, float farZ, float width, float height, float panX, float panY, float zoom)
    {
      float halfHeight = nearZ * tanf(fovY * PI / 360.0f) / zoom;
      float halfWidth = halfHeight * width / height;

      float offsetX = -panX * (halfWidth * 2 / width);
      float offsetY = -panY * (halfHeight * 2 / height);

      return glm::frustum(-halfWidth + offsetX, halfWidth + offsetX, -halfHeight + offsetY, halfHeight + offsetY, nearZ, farZ);
    }

    bool clip(Quad<XYZ.UV> &quad, const math::Rectf &clipRect, const glm::vec2 &textureFactor)
    {
      if ((quad.x1 > clipRect.x2 ) || (quad.x2 < clipRect.x1) || (quad.y1 > clipRect.y2) || (quad.y2 < clipRect.y1))
      {
        return false;
      }

      if (quad.x1 < clipRect.x1)
      {
        float dx = clipRect.x1 - quad.x1;
        quad.x1 += dx;
        quad.u1 += dx / textureFactor.x;
      }

      if (quad.x2 > clipRect.x2)
      {
        float dx = clipRect.x2 - quad.x2;
        quad.x2 += dx;
        quad.u2 += dx / textureFactor.x;
      }

      if (quad.y1 < clipRect.y1)
      {
        float dy = clipRect.y1 - quad.y1;
        quad.y1 += dy;
        quad.v1 += dy / textureFactor.y;
      }

      if (quad.y2 > clipRect.y2)
      {
        float dy = clipRect.y2 - quad.y2;
        quad.y2 += dy;
        quad.v2 += dy / textureFactor.y;
      }

      return true;
    }

    void addLineStrip(VertexBatch<XYZ> &batch, const std::vector<glm::vec2> &points)
    {
      for (const auto &point : points)
      {
        batch.addVertex(point);
      }
    }

    void addLines(VertexBatch<XYZ> &batch, const std::vector<glm::vec2> &points)
    {
      int size = points.size();
      if (size > 1)
      {
        for (int i = 0; i < size - 1; i++)
        {
          batch.addVertices(points[i], points[i + 1]);
        }
      }
    }
  }
}
