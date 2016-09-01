#pragma once

#include "chr/Platform.h"
#include "chr/Log.h"

#include <jni.h>
#include <android/asset_manager_jni.h>

namespace chr
{
  namespace android
  {
    extern AAssetManager *assetManager;
    extern std::string internalDataPath;
    extern std::string externalDataPath;
    extern std::string apkPath;

    void initFileSystem(JNIEnv *env, jobject context);
  }

  namespace jni
  {
    extern JavaVM *vm;
    
    // ---

    jint onLoad(JavaVM *vm, void *reserved);
    JNIEnv* getEnv();
    
    std::string toString(jstring s);
    jstring toJString(const std::string &s);
  }
}
