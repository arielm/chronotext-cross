#include "chr/image/Utils.h"
#include "chr/math/Utils.h"
#include "chr/MemoryBuffer.h"
#include "chr/Log.h"

#include <jpeglib.h>

using namespace std;

namespace chr
{
  namespace image
  {
    void readPngDataCallback(png_structp png_ptr, png_byte *raw_data, png_size_t read_length)
    {
      PngDataHandle *handle = reinterpret_cast<PngDataHandle*>(png_get_io_ptr(png_ptr));
      const png_byte *png_src = handle->data + handle->offset;

      memcpy(raw_data, png_src, read_length);
      handle->offset += read_length;
    }

    bool isPaletteGrayscale(const png_structp png_ptr, const png_infop info_ptr)
    {
      int num_palette;
      png_colorp palette;
      png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);

      for (int i = 0; i < num_palette; ++i)
      {
        if ((palette[i].red != palette[i].green) || (palette[i].red != palette[i].blue))
        {
          return false;
        }
      }

      return true;
    }

    ImageBuffer loadPngImage(const fs::path &relativePath, int flags)
    {
      bool ready = false;

      png_structp png_ptr;
      png_infop info_ptr;

      shared_ptr<MemoryBuffer> memoryBuffer;
      FILE *fd = nullptr;

      ImageBuffer image;

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
                image::PngDataHandle handle(memoryBuffer->data(), memoryBuffer->size());
                png_set_read_fn(png_ptr, &handle, image::readPngDataCallback);

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
        fd = fopen(getResourceFilePath(relativePath).string().data(), "rb");

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
        png_uint_32 width, height;
        int bit_depth, color_type;

        png_read_info(png_ptr, info_ptr);
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

        bool forceTranslucent = (flags & FLAGS_TRANSLUCENT) | (flags & FLAGS_TRANSLUCENT_INVERSE);

        switch (color_type)
        {
          case PNG_COLOR_TYPE_GRAY:
          {
            image.components = 1;
          }
          break;

          case PNG_COLOR_TYPE_PALETTE:
          {
            png_set_palette_to_rgb(png_ptr);

            if (forceTranslucent)
            {
              if (image::isPaletteGrayscale(png_ptr, info_ptr))
              {
                png_set_rgb_to_gray(png_ptr, 2, 0, 0);
              }
              else
              {
                png_set_rgb_to_gray(png_ptr, 1, -1, -1);
              }

              image.components = 1;
            }
            else
            {
              image.components = 3;
            }
          }
          break;

          case PNG_COLOR_TYPE_RGB:
          {
            if (forceTranslucent)
            {
              png_set_rgb_to_gray(png_ptr, 1, -1, -1);
              image.components = 1;
            }
            else
            {
              image.components = 3;
            }
          }
          break;

          case PNG_COLOR_TYPE_RGB_ALPHA:
          {
            image.components = 4;
          }
          break;
        }

        if ((flags & FLAGS_TRANSLUCENT_INVERSE) && (image.components == 1))
        {
          png_set_invert_mono(png_ptr);
        }
        else if ((flags & FLAGS_RBGA) && (image.components == 3))
        {
          png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
          image.components = 4;
        }

        png_read_update_info(png_ptr, info_ptr);

        // ---

        image.innerWidth = width;
        image.innerHeight = height;

        if (flags & FLAGS_POT)
        {
          image.width = math::nextPowerOfTwo(width);
          image.height = math::nextPowerOfTwo(height);
        }
        else
        {
          image.width = width;
          image.height = height;
        }

        if (png_get_rowbytes(png_ptr, info_ptr) == image.components * width)
        {
          image.buffer = shared_ptr<uint8_t>(new uint8_t[image.components * image.width * image.height], boost::checked_array_deleter<uint8_t>());
          auto data = image.buffer.get();

          png_byte *rows[height];

          for (auto i = 0; i < height; i++)
          {
          rows[i] = data + i * image.components * image.width;
          }

          png_read_image(png_ptr, &rows[0]);
        }

        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      }

      return image;
    }

    ImageBuffer loadJpgImage(const fs::path &relativePath, int flags)
    {
      struct jpeg_decompress_struct cinfo;
      struct jpeg_error_mgr jerr;

      cinfo.err = jpeg_std_error(&jerr);
      jpeg_create_decompress(&cinfo);

      shared_ptr<MemoryBuffer> memoryBuffer;
      FILE *fd = nullptr;

      ImageBuffer image;

      if (hasMemoryResources())
      {
        memoryBuffer = getResourceBuffer(relativePath);

        if (memoryBuffer)
        {
          jpeg_mem_src(&cinfo, reinterpret_cast<const unsigned char *>(memoryBuffer->data()), memoryBuffer->size());
        }
      }
      else if (hasFileResources())
      {
        fd = fopen(getResourceFilePath(relativePath).string().data(), "rb");

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
          bool forceTranslucent = (flags & FLAGS_TRANSLUCENT) | (flags & FLAGS_TRANSLUCENT_INVERSE);

          if (forceTranslucent)
          {
            image.components = 1;
          }
          else if (flags & FLAGS_RBGA)
          {
            image.components = 4;
          }
          else
          {
            image.components = 3;
          }

          // ---

          image.innerWidth = cinfo.image_width;
          image.innerHeight = cinfo.image_height;

          if (flags & FLAGS_POT)
          {
            image.width = math::nextPowerOfTwo(cinfo.image_width);
            image.height = math::nextPowerOfTwo(cinfo.image_height);
          }
          else
          {
            image.width = cinfo.image_width;
            image.height = cinfo.image_height;
          }

          image.buffer = shared_ptr<uint8_t>(new uint8_t[image.components * image.width * image.height], boost::checked_array_deleter<uint8_t>());
          auto data = image.buffer.get();

          jpeg_start_decompress(&cinfo);

          if (image.components == 3)
          {
            while (cinfo.output_scanline < cinfo.output_height)
            {
              uint8_t *line = data + (3 * image.width) * cinfo.output_scanline;
              jpeg_read_scanlines(&cinfo, &line, 1);
            }
          }
          else
          {
            auto lineBuffer = make_unique<uint8_t[]>(3 * cinfo.image_width);
            auto line = lineBuffer.get();

            while (cinfo.output_scanline < cinfo.output_height)
            {
              uint8_t *input = lineBuffer.get();
              uint8_t *output = data + (image.components * image.width) * cinfo.output_scanline;
              jpeg_read_scanlines(&cinfo, &line, 1);

              if (image.components == 4)
              {
                for (int i = 0; i < cinfo.image_width; i++)
                {
                  *output++ = *input++;
                  *output++ = *input++;
                  *output++ = *input++;
                  *output++ = 0xff;
                }
              }
              else if (image.components == 1)
              {
                if (flags & FLAGS_TRANSLUCENT_INVERSE)
                {
                  for (int i = 0; i < cinfo.image_width; i++)
                  {
                    int r = *input++;
                    int g = *input++;
                    int b = *input++;
                    *output++ = 0xff - (r + g + b) / 3;
                  }
                }
                else
                {
                  for (int i = 0; i < cinfo.image_width; i++)
                  {
                    int r = *input++;
                    int g = *input++;
                    int b = *input++;
                    *output++ = (r + g + b) / 3;
                  }
                }
              }
            }
          }

          jpeg_finish_decompress(&cinfo);
          jpeg_destroy_decompress(&cinfo);

          if (fd)
          {
            fclose(fd);
          }
        }
      }

      return  image;
    }

    ImageBuffer loadImage(const fs::path &relativePath, int flags)
    {
      ImageBuffer image;

      if (relativePath.extension() == ".png")
      {
        image = image::loadPngImage(relativePath, flags);
      }
      else if ((relativePath.extension() == ".jpg") || (relativePath.extension() == ".jpeg"))
      {
        image = image::loadJpgImage(relativePath, flags);
      }

      if (!image.isValid())
      {
        LOGE << "UNABLE TO LOAD IMAGE: " << relativePath.filename().string() << endl;
      }

      return image;
    }
  }
}
