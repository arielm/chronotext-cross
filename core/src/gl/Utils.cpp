#include "gl/Utils.h"
#include "gl/TextureInfo.h"
#include "math/Utils.h"

#include "Log.h"
#include "MemoryBuffer.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include "stb_image.h"

#include <jpeglib.h>
#include <png.h>

using namespace std;

namespace chr
{
  namespace gl
  {
    struct PngDataHandle
    {
        const png_byte *data = nullptr;
        png_size_t size = 0;
        png_size_t offset = 0;

        PngDataHandle(const void *data, size_t size)
        :
        data(reinterpret_cast<const png_byte*>(data)),
        size(size)
        {}
    };

    static void readPngDataCallback(png_structp png_ptr, png_byte *raw_data, png_size_t read_length)
    {
      PngDataHandle *handle = reinterpret_cast<PngDataHandle*>(png_get_io_ptr(png_ptr));
      const png_byte *png_src = handle->data + handle->offset;

      memcpy(raw_data, png_src, read_length);
      handle->offset += read_length;
    }

    TextureInfo loadPngTexture(const fs::path &relativePath, bool forceAlpha)
    {
      TextureInfo result;
      bool ready = false;

      png_structp png_ptr;
      png_infop info_ptr;

      shared_ptr<MemoryBuffer> memoryBuffer;
      FILE *fd = nullptr;

      if (hasMemoryResources())
      {
        memoryBuffer = getResourceBuffer(relativePath);

        if (memoryBuffer)
        {
          if ((memoryBuffer->size() > 8) && png_check_sig(reinterpret_cast<png_const_bytep>(memoryBuffer->data()), 8))
          {
            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

            if (png_ptr)
            {
              info_ptr = png_create_info_struct(png_ptr);

              if (info_ptr)
              {
                PngDataHandle handle(memoryBuffer->data(), memoryBuffer->size());
                png_set_read_fn(png_ptr, &handle, readPngDataCallback);

                if (!setjmp(png_jmpbuf(png_ptr)))
                {
                  png_set_sig_bytes(png_ptr, 0);
                  ready = true;
                }
              }
              else
              {
                png_destroy_read_struct(&png_ptr, NULL, NULL);
              }
            }
          }
        }
      }
      else if (hasFileResources())
      {
        fd = fopen(getResourcePath(relativePath).string().data(), "rb");

        if (fd)
        {
          uint8_t header[8];
          fread(header, 1, 8, fd);

          if (!png_sig_cmp(header, 0, 8))
          {
            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

            if (png_ptr)
            {
              info_ptr = png_create_info_struct(png_ptr);

              if (info_ptr)
              {
                if (!setjmp(png_jmpbuf(png_ptr)))
                {
                  png_init_io(png_ptr, fd);
                  png_set_sig_bytes(png_ptr, 8);

                  ready = true;
                }
              }
              else
              {
                png_destroy_read_struct(&png_ptr, NULL, NULL);
              }
            }
          }
        }
      }

      if (ready)
      {
        png_read_info(png_ptr, info_ptr);

        auto width = png_get_image_width(png_ptr, info_ptr);
        auto height = png_get_image_height(png_ptr, info_ptr);
        auto color_type = png_get_color_type(png_ptr, info_ptr);
        auto bit_depth = png_get_bit_depth(png_ptr, info_ptr);

        LOGI << width << "x" << height << endl;

        const png_size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
        auto buffer = make_unique<uint8_t[]>(row_size * height);
        auto data = buffer.get();

        png_byte *row_ptrs[height];

        for (auto i = 0; i < height; i++)
        {
          row_ptrs[i] = data + i * row_size;
        }

        png_read_image(png_ptr, &row_ptrs[0]);

        png_read_end(png_ptr, info_ptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

        // ---

        GLuint id = 0u;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        result.width = width;
        result.height = height;
        result.format = GL_RGB; // XXX
        result.id = id;

        uploadTextureData(result.format, result.width, result.height, data);

        if (fd)
        {
          fclose(fd);
        }
      }

      return result;
    }

    TextureInfo loadJpegTexture(const fs::path &relativePath, bool forceAlpha)
    {
      TextureInfo result;

      struct jpeg_decompress_struct cinfo;
      struct jpeg_error_mgr jerr;

      cinfo.err = jpeg_std_error(&jerr);
      jpeg_create_decompress(&cinfo);

      shared_ptr<MemoryBuffer> memoryBuffer;
      FILE *fd = nullptr;

      if (hasMemoryResources())
      {
        memoryBuffer = getResourceBuffer(relativePath);

        if (memoryBuffer)
        {
          jpeg_mem_src(&cinfo, reinterpret_cast<const unsigned char*>(memoryBuffer->data()), memoryBuffer->size());
        }
      }
      else if (hasFileResources())
      {
        fd = fopen(getResourcePath(relativePath).string().data(), "rb");

        if (fd)
        {
          jpeg_stdio_src(&cinfo, fd);
        }
      }

      if (memoryBuffer || fd)
      {
        jpeg_read_header(&cinfo, true);

        if ((cinfo.out_color_space == JCS_RGB) && (cinfo.num_components == 3))
        {
          auto buffer = make_unique<uint8_t[]>(cinfo.image_width * cinfo.image_height * 3);
          auto data = buffer.get();

          jpeg_start_decompress(&cinfo);

          while (cinfo.output_scanline < cinfo.output_height)
          {
            uint8_t *line = data + (3 * cinfo.image_width) * cinfo.output_scanline;
            jpeg_read_scanlines(&cinfo, &line, 1);
          }

          jpeg_finish_decompress(&cinfo);
          jpeg_destroy_decompress(&cinfo);

          if (fd)
          {
            fclose(fd);
          }

          // ---

          GLuint id = 0u;
          glGenTextures(1, &id);
          glBindTexture(GL_TEXTURE_2D, id);

          result.width = cinfo.image_width;
          result.height = cinfo.image_height;
          result.format = GL_RGB;
          result.id = id;

          uploadTextureData(result.format, result.width, result.height, data);
        }
      }

      return result;
    }

    TextureInfo loadTexture(const fs::path &relativePath, bool forceAlpha)
    {
      TextureInfo result;

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
        result.width = x;
        result.height = y;

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
          GLuint id = 0u;
          glGenTextures(1, &id);
          glBindTexture(GL_TEXTURE_2D, id);

          result.id = id;
          result.format = format;

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
