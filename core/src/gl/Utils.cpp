#include "gl/Utils.h"
#include "gl/TextureInfo.h"
#include "math/Utils.h"
#include "image/Utils.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    TextureInfo loadTexture(const fs::path &relativePath, int flags)
    {
      TextureInfo result;
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
            result.format = GL_ALPHA;
            break;

          case 3:
            result.format = GL_RGB;
            break;

          case 4:
            result.format = GL_RGBA;
            break;
        }

        if (result.format)
        {
          GLuint id = 0u;
          glGenTextures(1, &id);

          result.id = id;
          result.width = image.width;
          result.height = image.height;
          result.maxU = image.effectiveWidth / image.width;
          result.maxV = image.effectiveHeight / image.height;

          glBindTexture(GL_TEXTURE_2D, id);
          uploadTextureData(result.format, result.width, result.height, image.buffer.get());
        }
      }

      return result;
    }

    void uploadTextureData(GLenum format, GLsizei width, GLsizei height, const GLvoid *data)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

      #if defined(CHR_PLATFORM_DESKTOP)
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
      #endif

      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

      #if defined(CHR_PLATFORM_DESKTOP)
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
      #elif defined(CHR_PLATFORM_EMSCRIPTEN) || defined(CHR_PLATFORM_IOS) || defined(CHR_PLATFORM_ANDROID)
        glGenerateMipmap(GL_TEXTURE_2D);
      #endif
    }

    const glm::mat4 getPerspectiveMatrix(float fovy, float zNear, float zFar, float width, float height, float panX, float panY, float zoom)
    {
      float halfHeight = zNear * tanf(fovy * PI / 360.0f) / zoom;
      float halfWidth = halfHeight * width / height;

      float offsetX = -panX * (halfWidth * 2 / width);
      float offsetY = -panY * (halfHeight * 2 / height);

      return glm::frustum(-halfWidth + offsetX, halfWidth + offsetX, -halfHeight + offsetY, halfHeight + offsetY, zNear, zFar);
    }

    glm::vec3 transformPointAffine(const glm::mat4 &matrix, const glm::vec3 &point)
    {
      float x = matrix[0][0] * point.x + matrix[1][0] * point.y + matrix[2][0] * point.z + matrix[3][0];
      float y = matrix[0][1] * point.x + matrix[1][1] * point.y + matrix[2][1] * point.z + matrix[3][1];
      float z = matrix[0][2] * point.x + matrix[1][2] * point.y + matrix[2][2] * point.z + matrix[3][2];

      return glm::vec3(x, y, z);
    }
  }
}
