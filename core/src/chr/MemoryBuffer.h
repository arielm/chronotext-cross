#pragma once

#include "chr/Platform.h"
#include "chr/FileSystem.h"

namespace chr
{
  class MemoryBuffer
  {
  public:
    ~MemoryBuffer();

    bool lock(const fs::path &relativePath);
    void unlock();
            
    size_t size() const;
    const void* data() const;

  protected:
    size_t _size = 0;
    const void *_data = nullptr;
    bool locked = false;

  #if defined(CHR_FS_APK)
    AAsset *asset = nullptr;
  #endif
  };

  /*
   * THE FOLLOWING IS BASED ON: http://stackoverflow.com/a/13059195/50335
   */

  struct membuf : std::streambuf
  {
    membuf(char const* base, size_t size)
    {
      char* p(const_cast<char*>(base));
      this->setg(p, p, p + size);
    }
  };

  struct imemstream : virtual membuf, std::istream
  {
    imemstream(const void* base, size_t size)
    :
    membuf(static_cast<char const*>(base), size),
    std::istream(static_cast<std::streambuf*>(this))
    {}

    imemstream(std::shared_ptr<MemoryBuffer> memoryBuffer)
    :
    membuf(static_cast<char const*>(memoryBuffer->data()), memoryBuffer->size()),
    std::istream(static_cast<std::streambuf*>(this))
    {}
  };
}
