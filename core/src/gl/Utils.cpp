#include "gl/Utils.h"
#include "Log.h"
#include "MemoryBuffer.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include "stb_image.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    GLuint loadTexture(const fs::path &relativePath, bool forceAlpha)
    {
      GLuint texture = 0u;

      stbi_uc *data = nullptr;
      int x, y, comp;

      if (hasMemoryResources())
      {
        auto memoryBuffer = getResourceBuffer(relativePath);

        if (memoryBuffer)
        {
          data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(memoryBuffer->data()), memoryBuffer->size(), &x, &y, &comp, 0);
        }
      }
      else if (hasFileResources())
      {
        auto resPath = getResourcePath(relativePath);
        data = stbi_load(resPath.string().data(), &x, &y, &comp, 0);
      }

      if (data)
      {
        GLenum format = 0;

        switch (comp)
        {
          case 1:
            format = GL_ALPHA;
            break;

          case 3:
            format = GL_RGB;
            break;

          case 4:
            format = GL_RGBA;
            break;
        }

        if (format)
        {
          glGenTextures(1, &texture);
          glBindTexture(GL_TEXTURE_2D, texture);

          if (forceAlpha && (format != GL_ALPHA))
          {
            int size = x * y;
            int offset = comp - 1;

            auto converted = make_unique<uint8_t[]>(size);
            auto buffer = converted.get();

            for (auto i = 0; i < size; i++, offset += comp)
            {
              *buffer++ = data[offset];
            }

            uploadTextureData(GL_ALPHA, x, y, converted.get());
          }
          else
          {
            uploadTextureData(format, x, y, data);
          }
        }
        else
        {
          LOGE << "UNSUPPORTED IMAGE FORMAT" << endl;
        }

        stbi_image_free(data);
      }
      else
      {
        LOGE << "ERROR WHILE LOADING IMAGE" << endl;
      }

      return texture;
    }

    void uploadTextureData(GLenum format, GLsizei width, GLsizei height, const GLvoid *data)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

      #if defined(CHR_PLATFORM_DESKTOP)
        glHint(0x8192, GL_NICEST); // GL_GENERATE_MIPMAP_HINT
        glTexParameteri(GL_TEXTURE_2D, 0x8191, GL_TRUE); // GL_GENERATE_MIPMAP
      #endif

      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

      #if defined(CHR_PLATFORM_DESKTOP)
        glTexParameteri(GL_TEXTURE_2D, 0x8191, GL_FALSE); // GL_GENERATE_MIPMAP
      #elif defined(CHR_PLATFORM_EMSCRIPTEN) || defined(CHR_PLATFORM_IOS) || defined(CHR_PLATFORM_ANDROID)
        glGenerateMipmap(GL_TEXTURE_2D);
      #endif
    }
  }
}
