#include "Platform.h"

#if defined(CHR_FS_APK)
# include "Bridge.h"
#endif

using namespace std;

namespace chr
{
  bool hasFileResources()
  {
#if defined(CHR_FS_APK) || defined(CHR_FS_RC)
    return false;
#else
    return true;
#endif
  }

  fs::path getResourcePath(const fs::path &relativePath)
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
              
    basePath = fs::path(buffer) / "res";
              
    CFRelease(url);
    CFRelease(tmp);
    free(buffer);
#elif defined(CHR_PLATFORM_ANDROID)
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

#if defined(CHR_FS_APK)
  string toString(JNIEnv *env, jstring s)
  {
    string result;

    if (s)
    {
      const char *chars = env->GetStringUTFChars(s, nullptr);
                
      if (chars)
      {
        result.assign(chars);
        env->ReleaseStringUTFChars(s, chars);
      }
    }

    return result;
  }

  vector<string> toStrings(JNIEnv *env, jobjectArray a)
  {
    vector<string> result;

    auto size = env->GetArrayLength(a);
    result.reserve(size);

    for (auto i = 0; i < size; i++)
    {
      result.emplace_back(chr::toString(env, (jstring)env->GetObjectArrayElement(a, i)));
    }

    return result;
  }
#endif

#if defined(CHR_FS_RC)
  int checkResource(int resId)
  {
	  HRSRC infoHandle = ::FindResource(NULL, MAKEINTRESOURCE(resId), RT_RCDATA);

	  if (infoHandle)
	  {
      return ::SizeofResource(NULL, infoHandle);
	  }

   return ::GetLastError();
  }

  int checkResource(const fs::path &relativePath)
  {
    auto basePath = fs::path("res") / relativePath;
    auto found = mingw::RESOURCES.find(basePath.generic_string());

    if (found != mingw::RESOURCES.end())
    {
      int resId = found->second;
      return checkResource(resId);
    }

    return -1;
  }
#elif defined(CHR_FS_APK)
  int checkResource(const fs::path &relativePath)
  {
    AAsset *asset = AAssetManager_open(android::assetManager, relativePath.c_str(), AASSET_MODE_UNKNOWN);

    if (asset)
    {
      auto size = AAsset_getLength(asset);
      AAsset_close(asset);

      return size;
    }

    return -1;
  }
#endif
}
