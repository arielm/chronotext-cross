#include "chr/FileSystem.h"
#include "chr/MemoryBuffer.h"

using namespace std;

namespace chr
{
  bool hasFileResources()
  {
    #if defined(CHR_FS_APK) || defined(CHR_FS_RC) || defined(CHR_FS_JS_EMBED) || defined(CHR_FS_JS_PRELOAD)
      return false;
    #else
      return true;
    #endif
  }

  bool hasMemoryResources()
  {
    #if defined(CHR_FS_APK) || defined(CHR_FS_RC) || defined(CHR_FS_JS_EMBED) || defined(CHR_FS_JS_PRELOAD)
      return true;
    #else
      return false;
    #endif
  }

  fs::path getResourceFilePath(const fs::path &relativePath)
  {
    fs::path basePath;

    #if defined(CHR_FS_APK) || defined(CHR_FS_RC)
      return "";
    #elif defined(CHR_FS_BUNDLE)
      CFBundleRef bundle = CFBundleGetMainBundle();
      CFURLRef url = CFBundleCopyBundleURL(bundle);
      CFStringRef tmp = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);

      CFIndex length = CFStringGetLength(tmp);
      CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8);
      char *buffer = (char*)malloc(maxSize);
      CFStringGetCString(tmp, buffer, maxSize, kCFStringEncodingUTF8);
                
      basePath = fs::path(buffer);
                
      CFRelease(url);
      CFRelease(tmp);
      free(buffer);
    #elif defined(CHR_PLATFORM_ANDROID) // TODO: CONSIDER "GENERALIZING" TO LINUX
      static char buf[PATH_MAX];
      auto len = readlink("/proc/self/exe", buf, PATH_MAX - 1);
      assert(len > 0);
      basePath = fs::path(string(buf, len)).parent_path();
    #elif defined(CHR_PLATFORM_EMSCRIPTEN)
      basePath = "res";
    #else
      basePath = fs::current_path() / "res";
    #endif

    return basePath / relativePath;
  }

  shared_ptr<MemoryBuffer> getResourceBuffer(const fs::path &relativePath)
  {
    #if !defined(CHR_FS_APK) && !defined(CHR_FS_RC) && !defined(CHR_FS_JS_EMBED) && !defined(CHR_FS_JS_PRELOAD)
      return nullptr;
    #endif

    auto buffer = make_shared<MemoryBuffer>();
    buffer->lock(relativePath);

    return buffer;
  }

  shared_ptr<istream> getResourceStream(const fs::path &relativePath)
  {
    istream *stream;

    if (chr::hasMemoryResources())
    {
      auto memoryBuffer = chr::getResourceBuffer(relativePath);

      if (memoryBuffer)
      {
        stream = new imemstream(memoryBuffer);
      }
      else
      {
        return nullptr;
      }
    }
    else if (chr::hasFileResources())
    {
      stream = new fs::ifstream(chr::getResourceFilePath(relativePath), ifstream::binary);
    }

    return shared_ptr<istream>(stream);
  }
}
