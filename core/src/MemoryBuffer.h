#pragma once

#include "Platform.h"

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
}
