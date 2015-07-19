#pragma once

#include <vector>
#include <map>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;

namespace chr
{
  enum
  {
    PLATFORM_OSX,
    PLATFORM_IOS,
    PLATFORM_IOS_SIM,
    PLATFORM_ANDROID,
    PLATFORM_EMSCRIPTEN,
    PLATFORM_MINGW,
  };
}

#if defined(__APPLE__)
  #include <TargetConditionals.h>
  #define CHR_PLATFORM_COCOA
  #if TARGET_OS_IPHONE
    #if TARGET_IPHONE_SIMULATOR
      #define CHR_PLATFORM chr::PLATFORM_IOS_SIM
      #define CHR_PLATFORM_IOS_SIM 1
    #else
      #define CHR_PLATFORM chr::PLATFORM_IOS
      #define CHR_PLATFORM_IOS 1
    #endif // TARGET_IPHONE_SIMULATOR
  #else
    #define CHR_PLATFORM chr::PLATFORM_OSX
    #define CHR_PLATFORM_OSX 1
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
#else
  #error UNSUPPORTED PLATFORM
#endif // __APPLE__

#if defined(CHR_PLATFORM_MINGW)
  #include <windows.h>
  #undef ERROR // SEE https://google-glog.googlecode.com/svn/trunk/doc/glog.html#windows
#elif defined(CHR_PLATFORM_COCOA)
  #include <CoreFoundation/CoreFoundation.h>
#endif

#if defined(CHR_FS_APK)
  #include <android/asset_manager_jni.h>
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
  bool hasFileResources();
  fs::path getResourcePath(const fs::path &relativePath);

#if defined(CHR_RUN_APK)
  std::string toString(JNIEnv *env, jstring s);
  std::vector<std::string> toStrings(JNIEnv *env, jobjectArray a);
#endif

#if defined(CHR_FS_RC)
  int checkResource(int resId);
  int checkResource(const fs::path &relativePath);
#elif defined(CHR_FS_APK)
  int checkResource(const fs::path &relativePath);
#endif
}
