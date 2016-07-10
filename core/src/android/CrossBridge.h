#pragma once

#include "android/JNI.h"

namespace chr
{
  namespace jni
  {
    extern jobject bridge;
    
    // ---
    
    void callVoidMethodOnBridge(const char *name, const char *sig, ...);
    jboolean callBooleanMethodOnBridge(const char *name, const char *sig, ...);
    jchar callCharMethodOnBridge(const char *name, const char *sig, ...);
    jint callIntMethodOnBridge(const char *name, const char *sig, ...);
    jlong callLongMethodOnBridge(const char *name, const char *sig, ...);
    jfloat callFloatMethodOnBridge(const char *name, const char *sig, ...);
    jdouble callDoubleMethodOnBridge(const char *name, const char *sig, ...);
    jobject callObjectMethodOnBridge(const char *name, const char *sig, ...);
  }

  namespace bridge
  {
    void init(JNIEnv *env, jobject obj, jobject bridge, jobject context, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity);
    void uninit(JNIEnv *env, jobject obj);
    void setup(JNIEnv *env, jobject obj, jint width, jint height);
    void shutdown(JNIEnv *env, jobject obj);

    void resize(JNIEnv *env, jobject obj, jint width, jint height);
    void draw(JNIEnv *env, jobject obj);

    void addTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y);
    void updateTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y);
    void removeTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y);

    void dispatchEvent(JNIEnv *env, jobject obj, jint eventId);
    void sendMessageToSketch(JNIEnv *env, jobject obj, jint what, jstring body);
  }
}
