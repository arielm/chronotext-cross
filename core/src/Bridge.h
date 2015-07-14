#pragma once

#include <string>

#include <android/asset_manager_jni.h>

extern "C"
{
  jint JNI_OnLoad(JavaVM *vm, void *reserved);

  void Java_org_chronotext_Bridge_performInit(JNIEnv *env, jobject obj, jobject activity);
  jint Java_org_chronotext_Bridge_invokeMain(JNIEnv *env, jobject obj, jobjectArray args);
}

namespace chr
{
  namespace android
  {
    extern JavaVM *vm;
    extern jobject activity;

    extern AAssetManager *assetManager;
    extern std::string internalDataPath;
    extern std::string externalDataPath;
    extern std::string apkPath;
  }
}

extern int main(int argc, const char *argv[]);
