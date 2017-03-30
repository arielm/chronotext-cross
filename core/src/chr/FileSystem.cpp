#if defined(CHR_PLATFORM_OSX) || defined(CHR_PLATFORM_IOS)
  #if !defined(__OBJC__)
    #error THIS FILE MUST BE COMPILED AS OBJECTIVE-C++
  #endif
#endif

#include "chr/FileSystem.h"
#include "chr/ResourceBuffer.h"
#include "chr/cross/Context.h"

#if defined(CHR_PLATFORM_OSX) || defined(CHR_PLATFORM_IOS)
  #import <Foundation/Foundation.h>
#elif defined(CHR_PLATFORM_WIN)
  #include <windows.h>
  #include <shlobj.h>
#endif

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
      if (fs::exists(fs::current_path() / relativePath))
      {
        return fs::current_path() / relativePath; // RELEVANT WHEN BUILDING OSX APP WITH XCODE
      }
      else
      {
        return fs::current_path() / "res" / relativePath; // RELEVANT WHEN BUILDING WITH CLION OR COMMAND-LINE
      }
    #endif

    return basePath / relativePath;
  }

  shared_ptr<ResourceBuffer> getResourceBuffer(const fs::path &relativePath)
  {
    #if !defined(CHR_FS_APK) && !defined(CHR_FS_RC) && !defined(CHR_FS_JS_EMBED) && !defined(CHR_FS_JS_PRELOAD)
      return nullptr;
    #endif

    auto buffer = make_shared<ResourceBuffer>();
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
        stream = new imemstream(memoryBuffer); // FIXME: PROBLEMATIC BECAUSE memoryBuffer WILL BE DEALLOCATED
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

  fs::path getDocumentsFolder()
  {
    #if defined(CHR_PLATFORM_OSX)
      NSString *path = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
      return fs::path([path cStringUsingEncoding:NSUTF8StringEncoding]);
    #elif defined(CHR_PLATFORM_WIN)
      char buffer[MAX_PATH];
      SHGetFolderPathA(0, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, buffer);
      return fs::path(string(buffer));
    #elif defined(CHR_PLATFORM_IOS)
      NSString *path = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
      return fs::path([path cStringUsingEncoding:NSUTF8StringEncoding]);
    #elif defined(CHR_PLATFORM_ANDROID)
      auto documentsFolder = fs::path(chr::android::externalDataPath) / "Documents";

      if (!boost::filesystem::exists(documentsFolder))
      {
        boost::filesystem::create_directory(documentsFolder);
      }

      return documentsFolder;
    #endif

    return "";
  }
}
