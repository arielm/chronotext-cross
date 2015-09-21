/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "android/JNI.h"
#include "cross/Context.h"

using namespace std;

namespace chr
{
    namespace android
    {
        AAssetManager *assetManager = nullptr;
        string internalDataPath;
        string externalDataPath;
        string apkPath;
    }

    namespace jni
    {
        JavaVM *vm = nullptr;
        
        jobject bridge = nullptr;
        CrossDelegate *crossDelegate = nullptr;
        
        // ---
        
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
        
        // ---
        
        /*
         * TODO: THROW "JNI EXCEPTION" WITH THE RELEVANT JAVA ERROR MESSAGE
         *
         * - IF METHOD CAN'T BE FOUND
         * - IF CALL FAILED
         *
         * HINTS: http://stackoverflow.com/questions/6015293/convert-exceptiondescribe-to-string
         */
        
        void callVoidMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            env->CallVoidMethodV(bridge, method, args);
            va_end(args);
        }
        
        jboolean callBooleanMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jboolean ret = env->CallBooleanMethodV(bridge, method, args);
            va_end(args);
            
            return ret;
        }
        
        jchar callCharMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jchar ret = env->CallCharMethodV(bridge, method, args);
            va_end(args);
            
            return ret;
        }
        
        jint callIntMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jint ret = env->CallIntMethodV(bridge, method, args);
            va_end(args);
            
            return ret;
        }
        
        jlong callLongMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jlong ret = env->CallLongMethodV(bridge, method, args);
            va_end(args);
            
            return ret;
        }
        
        jfloat callFloatMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jfloat ret = env->CallFloatMethodV(bridge, method, args);
            va_end(args);
            
            return ret;
        }
        
        jdouble callDoubleMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jdouble ret = env->CallDoubleMethod(bridge, method, args);
            va_end(args);
            
            return ret;
        }
        
        jobject callObjectMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jobject ret = env->CallObjectMethodV(bridge, method, args);
            va_end(args);
            
            return ret;
        }
    }
}

#pragma mark ---------------------------------------- JAVA <-> C++ ----------------------------------------

using namespace chr;

/*
 * WARNING: THIS IS *NOT* NECESSARILY CALLED EACH TIME THE APPLICATION STARTS...
 */
jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    jni::vm = vm; // ...THEREFORE: THIS SHOULD *NOT* BE CLEARED
    
    return JNI_VERSION_1_6;
}

// ---

void Java_org_chronotext_cross_CrossBridge_init(JNIEnv *env, jobject obj, jobject bridge, jobject context, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity)
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

    // ---

    jni::bridge = env->NewGlobalRef(bridge);
    
    jni::crossDelegate = new CrossDelegate();
    jni::crossDelegate->performInit(env, context, display, glm::vec2(displayWidth, displayHeight), displayDensity);
}

void Java_org_chronotext_cross_CrossBridge_uninit(JNIEnv *env, jobject obj)
{
    jni::crossDelegate->performUninit(env);
    delete jni::crossDelegate;
    
    env->DeleteGlobalRef(jni::bridge);
    jni::bridge = nullptr;
    
    // CI_LOGV("UNINIT");
}

void Java_org_chronotext_cross_CrossRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height)
{
    jni::crossDelegate->performSetup(env, glm::vec2(width, height));
}

void Java_org_chronotext_cross_CrossRenderer_shutdown(JNIEnv *env, jobject obj)
{
    jni::crossDelegate->performShutdown(env);
}

// ---

void Java_org_chronotext_cross_CrossRenderer_resize(JNIEnv *env, jobject obj, jint width, jint height)
{
    jni::crossDelegate->performResize(glm::vec2(width, height));
}

void Java_org_chronotext_cross_CrossRenderer_draw(JNIEnv *env, jobject obj)
{
    jni::crossDelegate->performUpdate();
    jni::crossDelegate->performDraw();
}

// ---

void Java_org_chronotext_cross_CrossRenderer_addTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    jni::crossDelegate->addTouch(index, x, y);
}

void Java_org_chronotext_cross_CrossRenderer_updateTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    jni::crossDelegate->updateTouch(index, x, y);
}

void Java_org_chronotext_cross_CrossRenderer_removeTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    jni::crossDelegate->removeTouch(index, x, y);
}

// ---

void Java_org_chronotext_cross_CrossRenderer_dispatchEvent(JNIEnv *env, jobject obj, jint eventId)
{
    jni::crossDelegate->handleEvent(eventId);
}

void Java_org_chronotext_cross_CrossBridge_sendMessageToSketch(JNIEnv *env, jobject obj, jint what, jstring body)
{
    if (body)
    {
        const char *chars = env->GetStringUTFChars(body, nullptr);
        jni::crossDelegate->messageFromBridge(what, chars);
        env->ReleaseStringUTFChars(body, chars);
    }
    else
    {
        jni::crossDelegate->messageFromBridge(what);
    }
}
