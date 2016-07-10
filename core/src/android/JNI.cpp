#include "android/JNI.h"

using namespace std;

namespace chr
{
  namespace android
  {
    AAssetManager *assetManager = nullptr;
    string internalDataPath;
    string externalDataPath;
    string apkPath;

    void initFileSystem(JNIEnv *env, jobject context)
    {
      jmethodID getAssetsMethod = env->GetMethodID(env->GetObjectClass(context), "getAssets", "()Landroid/content/res/AssetManager;");
      android::assetManager = AAssetManager_fromJava(env, env->CallObjectMethod(context, getAssetsMethod));

      jmethodID getFilesDirMethod = env->GetMethodID(env->GetObjectClass(context), "getFilesDir", "()Ljava/io/File;");
      jobject filesDirObject = env->CallObjectMethod(context, getFilesDirMethod);
      jmethodID getAbsolutePathMethod = env->GetMethodID(env->GetObjectClass(filesDirObject), "getAbsolutePath", "()Ljava/lang/String;");
      jstring internalDataPath = (jstring)env->CallObjectMethod(filesDirObject, getAbsolutePathMethod);
      android::internalDataPath = jni::toString(internalDataPath);

      jclass environmentClass = env->FindClass("android/os/Environment");
      jmethodID getExternalStorageDirectoryMethod = env->GetStaticMethodID(environmentClass, "getExternalStorageDirectory", "()Ljava/io/File;");
      jobject externalStorageDirectoryObject = env->CallStaticObjectMethod(environmentClass, getExternalStorageDirectoryMethod);
      jstring externalDataPath = (jstring)env->CallObjectMethod(externalStorageDirectoryObject, getAbsolutePathMethod);
      android::externalDataPath = jni::toString(externalDataPath);

      jmethodID getPackageCodePathMethod = env->GetMethodID(env->GetObjectClass(context), "getPackageCodePath", "()Ljava/lang/String;");
      jstring apkPath = (jstring)env->CallObjectMethod(context, getPackageCodePathMethod);
      android::apkPath = jni::toString(apkPath);
    }
  }

  namespace jni
  {
    JavaVM *vm = nullptr;
    
    // ---

    /*
     * WARNING: THIS IS *NOT* NECESSARILY CALLED EACH TIME THE APPLICATION STARTS...
     */
    jint onLoad(JavaVM *vm, void *reserved)
    {
        chr::jni::vm = vm; // ...THEREFORE: THIS SHOULD *NOT* BE CLEARED

        return JNI_VERSION_1_6;
    }

    JNIEnv* getEnv()
    {
      JNIEnv *env = nullptr;
      auto err = vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
      
      if (err == JNI_EDETACHED)
      {
        LOGF << "CURRENT THREAD NOT ATTACHED TO JAVA VM" << endl;
      }
      else if (err == JNI_EVERSION)
      {
        LOGF << "VM DOESN'T SUPPORT REQUESTED JNI VERSION" << endl;
      }
      
      if (env)
      {
        return env;
      }
      
      LOGF << "INVALID JNI ENV" << endl;
    }
    
    // ---
    
    string toString(jstring s)
    {
      if (s)
      {
        JNIEnv *env = getEnv();
        
        const char *chars = env->GetStringUTFChars(s, nullptr);
        
        if (chars)
        {
          string tmp(chars);
          env->ReleaseStringUTFChars(s, chars);
          return tmp;
        }
      }
      
      return "";
    }
    
    jstring toJString(const string &s)
    {
      return getEnv()->NewStringUTF(s.data());
    }
  }
}
