#include "Bridge.h"
#include "Log.h"
#include "Platform.h"

using namespace std;

namespace chr
{
  namespace android
  {
    JavaVM *vm = nullptr;
    jobject activity = nullptr;

    AAssetManager *assetManager = nullptr;
    string internalDataPath;
    string externalDataPath;
    string apkPath;
  }
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
  LOGD << "JNI_Onload" << endl;

  JNIEnv *env;
  jint err = vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

  if (err != JNI_OK)
  {
    LOGF << "JNI ERROR: " << err << endl;
  }

  chr::android::vm = vm;

  return JNI_VERSION_1_6;
}

void Java_org_chronotext_Bridge_performInit(JNIEnv *env, jobject obj, jobject activity)
{
  chr::android::activity = env->NewGlobalRef(activity);

  jmethodID getAssetsMethod = env->GetMethodID(env->GetObjectClass(activity), "getAssets", "()Landroid/content/res/AssetManager;");
  chr::android::assetManager = AAssetManager_fromJava(env, env->CallObjectMethod(activity, getAssetsMethod));

  jmethodID getFilesDirMethod = env->GetMethodID(env->GetObjectClass(activity), "getFilesDir", "()Ljava/io/File;");
  jobject filesDirObject = env->CallObjectMethod(activity, getFilesDirMethod);
  jmethodID getAbsolutePathMethod = env->GetMethodID(env->GetObjectClass(filesDirObject), "getAbsolutePath", "()Ljava/lang/String;");
  jstring internalDataPath = (jstring)env->CallObjectMethod(filesDirObject, getAbsolutePathMethod);
  chr::android::internalDataPath = chr::toString(env, internalDataPath);

  jclass environmentClass = env->FindClass("android/os/Environment");
  jmethodID getExternalStorageDirectoryMethod = env->GetStaticMethodID(environmentClass, "getExternalStorageDirectory", "()Ljava/io/File;");
  jobject externalStorageDirectoryObject = env->CallStaticObjectMethod(environmentClass, getExternalStorageDirectoryMethod);
  jstring externalDataPath = (jstring)env->CallObjectMethod(externalStorageDirectoryObject, getAbsolutePathMethod);
  chr::android::externalDataPath = chr::toString(env, externalDataPath);

  jmethodID getPackageCodePathMethod = env->GetMethodID(env->GetObjectClass(activity), "getPackageCodePath", "()Ljava/lang/String;");
  jstring apkPath = (jstring)env->CallObjectMethod(activity, getPackageCodePathMethod);
  chr::android::apkPath = chr::toString(env, apkPath);
}

jint Java_org_chronotext_Bridge_invokeMain(JNIEnv *env, jobject obj, jobjectArray args)
{
  auto tmp = chr::toStrings(env, args);

  vector<const char*> argv;
  argv.emplace_back(chr::android::apkPath.data());
  
  for (const auto &arg : tmp)
  {
    argv.emplace_back(arg.data());
  }

  return main(argv.size(), argv.data());
}
