#include "chr/MemoryBuffer.h"

#if defined(CHR_FS_APK)
  #include "android/JNI.h"
#elif defined(FS_JS_EMBED) || defined(FS_JS_PRELOAD)
  #include <sys/mman.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include <fcntl.h>
#endif

using namespace std;

namespace chr
{
  MemoryBuffer::~MemoryBuffer()
  {
    unlock();
  }

  bool MemoryBuffer::lock(const fs::path &relativePath)
  {
    unlock();

    #if defined(CHR_FS_RC)
      if (!locked)
      {
        auto basePath = fs::path("res") / relativePath;
        auto found = win::RESOURCES.find(basePath.generic_string());

        if (found != win::RESOURCES.end())
        {
          int resId = found->second;
          HRSRC infoHandle = ::FindResource(NULL, MAKEINTRESOURCE(resId), RT_RCDATA);

          if (infoHandle)
          {
            HGLOBAL handle = ::LoadResource(NULL, infoHandle);

            if (handle)
            {
              _size = ::SizeofResource(NULL, infoHandle);
              _data = ::LockResource(handle);

              locked = true;
              return true;
            }
          }
        }
      }
    #elif defined(CHR_FS_APK)
      asset = AAssetManager_open(android::assetManager, relativePath.c_str(), AASSET_MODE_BUFFER);

      if (asset)
      {
        _size = AAsset_getLength(asset);
        _data = AAsset_getBuffer(asset);

        locked = true;
        return true;
      }
    #elif defined(FS_JS_EMBED) || defined(FS_JS_PRELOAD)
      auto fd = open(relativePath.c_str(), O_RDONLY);

      if (fd != -1)
      {
        struct stat stats;
        
        if ((fstat(fd, &stats) != -1) && (stats.st_size > 0))
        {
          _size = stats.st_size;
          _data = mmap(nullptr, size, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0);
          
          close(fd);
          
          if (_data != MAP_FAILED)
          {
            locked = true;
            return true;
          }
          
          _size = 0;
          _data = nullptr;
        }
      }
    #endif

    return false;
  }
        
  void MemoryBuffer::unlock()
  {
    #if defined(CHR_FS_APK)
      if (asset)
      {
        AAsset_close(asset);
        asset = nullptr;
      }
    #elif defined(FS_JS_EMBED) || defined(FS_JS_PRELOAD)
      if (_data && _size)
      {
        munmap(_data, _size);
      }
    #endif

    #if defined(CHR_FS_APK) || defined(FS_JS_EMBED) || defined(FS_JS_PRELOAD)
      _size = 0;
      _data = nullptr;

      locked = false;
    #endif
  }

  size_t MemoryBuffer::size() const
  {
    return _size;
  }

  const void* MemoryBuffer::data() const
  {
    return _data;
  }
}
