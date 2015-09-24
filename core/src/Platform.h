#pragma once

#include <vector>
#include <map>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace fs = boost::filesystem;

namespace chr
{
  class MemoryBuffer;

  enum
  {
    PLATFORM_OSX,
    PLATFORM_IOS,
    PLATFORM_ANDROID,
    PLATFORM_EMSCRIPTEN,
    PLATFORM_MINGW,
  };
}

#if defined(__APPLE__)
  #include <TargetConditionals.h>
  #define CHR_PLATFORM_COCOA
  #if TARGET_OS_IPHONE
    #define CHR_PLATFORM chr::PLATFORM_IOS
    #define CHR_PLATFORM_IOS 1
  #else
    #define CHR_PLATFORM chr::PLATFORM_OSX
    #define CHR_PLATFORM_OSX 1
    #define CHR_PLATFORM_DESKTOP 1
  #endif // TARGET_OS_IPHONE
#elif defined(__ANDROID__)
  #define CHR_PLATFORM chr::PLATFORM_ANDROID
  #define CHR_PLATFORM_ANDROID 1
#elif defined(__EMSCRIPTEN__)
  #define CHR_PLATFORM chr::PLATFORM_EMSCRIPTEN
  #define CHR_PLATFORM_EMSCRIPTEN 1
#elif defined(__MINGW32__)
  #define CHR_PLATFORM chr::PLATFORM_MINGW
  #define CHR_PLATFORM_MINGW 1
  #define CHR_PLATFORM_DESKTOP 1
#else
  #error UNSUPPORTED PLATFORM
#endif // __APPLE__

#if defined(CHR_PLATFORM_MINGW)
  #include <windows.h>
  #undef ERROR // SEE https://google-glog.googlecode.com/svn/trunk/doc/glog.html#windows
#elif defined(CHR_PLATFORM_COCOA)
  #include <CoreFoundation/CoreFoundation.h>
#elif defined(CHR_PLATFORM_ANDROID)
  #include <jni.h>
#endif

#if defined(CHR_FS_APK)
  #include <android/asset_manager.h>
#elif defined(CHR_FS_RC)
  namespace chr
  {
    namespace mingw
    {
      extern std::map<std::string, int> RESOURCES;
    }
  }
#endif

namespace chr
{
  namespace utils
  {
    /*
     * toString() AND split() BORROWED FROM:
     * https://github.com/cinder/Cinder/blob/master/include/cinder/Utilities.h
     */

    template<typename T>
    inline std::string toString(const T &t)
    {
      return boost::lexical_cast<std::string>(t);
    }

    std::vector<std::string> split(const std::string &str, char separator, bool compress = true);
    std::vector<std::string> split(const std::string &str, const std::string &separators, bool compress = true);

    uint64_t millisSinceEpoch();
  }

  bool hasFileResources();
  bool hasMemoryResources();

  fs::path getResourcePath(const fs::path &relativePath);
  std::shared_ptr<MemoryBuffer> getResourceBuffer(const fs::path &relativePath);
}
