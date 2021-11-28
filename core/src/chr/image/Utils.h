#pragma once

#include "chr/Platform.h"
#include "chr/FileSystem.h"
#include "chr/image/ImageBuffer.h"

#include <png.h>

namespace chr
{
  namespace image
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

  void readPngDataCallback(png_structp png_ptr, png_byte *raw_data, png_size_t read_length);
  bool isPaletteGrayscale(const png_structp png_ptr, const png_infop info_ptr);

  ImageBuffer loadPngImage(const InputSource &source, int flags = FLAGS_NONE);
  ImageBuffer loadJpgImage(const InputSource &source, int flags = FLAGS_NONE);
  ImageBuffer loadImage(const InputSource &source, int flags = FLAGS_NONE);
  }
}
