#include "gl/Utils.h"
#include "gl/TextureHandle.h"
#include "math/Utils.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    TextureHandle loadTexture(const fs::path &relativePath, int flags, bool useMipmap, GLenum wrapS, GLenum wrapT)
    {
      TextureHandle texture;
      image::ImageBuffer image;

      if (relativePath.extension() == ".png")
      {
        image = image::loadPngImage(relativePath, flags);
      }
      else if ((relativePath.extension() == ".jpg") || (relativePath.extension() == ".jpeg"))
      {
        image = image::loadJpgImage(relativePath, flags);
      }

      if (image.width * image.height)
      {
        switch (image.components)
        {
          case 1:
            texture.format = GL_ALPHA;
            break;

          case 3:
            texture.format = GL_RGB;
            break;

          case 4:
            texture.format = GL_RGBA;
            break;
        }

        if (texture.format)
        {
          GLuint id = 0u;
          glGenTextures(1, &id);

          texture.id = id;
          texture.width = image.width;
          texture.height = image.height;
          texture.maxU = image.effectiveWidth / (float) image.width;
          texture.maxV = image.effectiveHeight / (float) image.height;

          glBindTexture(GL_TEXTURE_2D, id);
          uploadTexture(texture.format, texture.width, texture.height, image.buffer.get(), useMipmap, wrapS, wrapT);
        }
      }

      return texture;
    }

    TextureHandle uploadMaskedTexture(const image::ImageBuffer &image, const image::ImageBuffer &mask, bool useMipmap, GLenum wrapS, GLenum wrapT)
    {
      TextureHandle texture;

      if ((image.components == 4) && (mask.components == 1))
      {
        if ((mask.width >= image.effectiveWidth) && (mask.height >= image.effectiveHeight))
        {
          auto input = mask.buffer.get();
          auto output = image.buffer.get();

          for (int iy = 0; iy < image.effectiveHeight; iy++)
          {
            uint8_t *inputLine = input + (mask.width) * iy;
            uint8_t *outputLine = output + (4 * image.width) * iy;

            for (int ix = 0; ix < image.effectiveWidth; ix++)
            {
              outputLine += 3;
              *outputLine ++= *inputLine++;
            }
          }
        }

        GLuint id = 0u;
        glGenTextures(1, &id);

        texture.id = id;
        texture.format = GL_RGBA;
        texture.width = image.width;
        texture.height = image.height;
        texture.maxU = image.effectiveWidth / (float) image.width;
        texture.maxV = image.effectiveHeight / (float) image.height;

        glBindTexture(GL_TEXTURE_2D, id);
        uploadTexture(texture.format, texture.width, texture.height, image.buffer.get(), useMipmap, wrapS, wrapT);
      }

      return  texture;
    }

    void uploadTexture(GLenum format, GLsizei width, GLsizei height, const GLvoid *data, bool useMipmap, GLenum wrapS, GLenum wrapT)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      if (useMipmap)
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        #if defined(CHR_PLATFORM_DESKTOP)
          glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
          glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        #endif
      }
      else
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      }

      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

      if (useMipmap)
      {
        #if defined(CHR_PLATFORM_DESKTOP)
          glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
        #elif defined(CHR_PLATFORM_EMSCRIPTEN) || defined(CHR_PLATFORM_IOS) || defined(CHR_PLATFORM_ANDROID)
          glGenerateMipmap(GL_TEXTURE_2D);
        #endif
      }
    }

    const glm::mat4 getPerspectiveMatrix(float fovy, float zNear, float zFar, float width, float height, float panX, float panY, float zoom)
    {
      float halfHeight = zNear * tanf(fovy * PI / 360.0f) / zoom;
      float halfWidth = halfHeight * width / height;

      float offsetX = -panX * (halfWidth * 2 / width);
      float offsetY = -panY * (halfHeight * 2 / height);

      return glm::frustum(-halfWidth + offsetX, halfWidth + offsetX, -halfHeight + offsetY, halfHeight + offsetY, zNear, zFar);
    }

    glm::vec3 transformPointAffine(const glm::mat4 &matrix, const glm::vec2 &input)
    {
      float x = matrix[0][0] * input.x + matrix[1][0] * input.y + matrix[3][0];
      float y = matrix[0][1] * input.x + matrix[1][1] * input.y + matrix[3][1];
      float z = matrix[0][2] * input.x + matrix[1][2] * input.y + matrix[3][2];

      return glm::vec3(x, y, z);
    }

    glm::vec3 transformPointAffine(const glm::mat4 &matrix, const glm::vec3 &input)
    {
      float x = matrix[0][0] * input.x + matrix[1][0] * input.y + matrix[2][0] * input.z + matrix[3][0];
      float y = matrix[0][1] * input.x + matrix[1][1] * input.y + matrix[2][1] * input.z + matrix[3][1];
      float z = matrix[0][2] * input.x + matrix[1][2] * input.y + matrix[2][2] * input.z + matrix[3][2];

      return glm::vec3(x, y, z);
    }

    template <>
    void transformQuadAffine<GL_TRIANGLE_STRIP, GL_CCW>(const glm::mat4 &matrix, float x1, float y1, float x2, float y2, vector<glm::vec3> &output)
    {
      float x100 = x1 * matrix[0][0];
      float x110 = x1 * matrix[0][1];
      float x120 = x1 * matrix[0][2];

      float y101 = y1 * matrix[1][0];
      float y111 = y1 * matrix[1][1];
      float y121 = y1 * matrix[1][2];

      float x200 = x2 * matrix[0][0];
      float x210 = x2 * matrix[0][1];
      float x220 = x2 * matrix[0][2];

      float y201 = y2 * matrix[1][0];
      float y211 = y2 * matrix[1][1];
      float y221 = y2 * matrix[1][2];

      output.emplace_back(x100 + y101 + matrix[3][0], x110 + y111 + matrix[3][1], x120 + y121 + matrix[3][2]); // x1, y1
      output.emplace_back(x100 + y201 + matrix[3][0], x110 + y211 + matrix[3][1], x120 + y221 + matrix[3][2]); // x1, y2
      output.emplace_back(x200 + y101 + matrix[3][0], x210 + y111 + matrix[3][1], x220 + y121 + matrix[3][2]); // x2, y1
      output.emplace_back(x200 + y201 + matrix[3][0], x210 + y211 + matrix[3][1], x220 + y221 + matrix[3][2]); // x2, y2
    }

    template <>
    void transformQuadAffine<GL_TRIANGLE_STRIP, GL_CW>(const glm::mat4 &matrix, float x1, float y1, float x2, float y2, vector<glm::vec3> &output)
    {
      float x100 = x1 * matrix[0][0];
      float x110 = x1 * matrix[0][1];
      float x120 = x1 * matrix[0][2];

      float y101 = y1 * matrix[1][0];
      float y111 = y1 * matrix[1][1];
      float y121 = y1 * matrix[1][2];

      float x200 = x2 * matrix[0][0];
      float x210 = x2 * matrix[0][1];
      float x220 = x2 * matrix[0][2];

      float y201 = y2 * matrix[1][0];
      float y211 = y2 * matrix[1][1];
      float y221 = y2 * matrix[1][2];

      output.emplace_back(x100 + y101 + matrix[3][0], x110 + y111 + matrix[3][1], x120 + y121 + matrix[3][2]); // x1, y1
      output.emplace_back(x200 + y101 + matrix[3][0], x210 + y111 + matrix[3][1], x220 + y121 + matrix[3][2]); // x2, y1
      output.emplace_back(x100 + y201 + matrix[3][0], x110 + y211 + matrix[3][1], x120 + y221 + matrix[3][2]); // x1, y2
      output.emplace_back(x200 + y201 + matrix[3][0], x210 + y211 + matrix[3][1], x220 + y221 + matrix[3][2]); // x2, y2
    }
  }
}
