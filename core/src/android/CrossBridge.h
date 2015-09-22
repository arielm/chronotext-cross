/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "android/JNI.h"

extern "C"
{
    void Java_org_chronotext_cross_CrossBridge_init(JNIEnv *env, jobject obj, jobject bridge, jobject context, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity);
    void Java_org_chronotext_cross_CrossBridge_uninit(JNIEnv *env, jobject obj);
    void Java_org_chronotext_cross_CrossRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height);
    void Java_org_chronotext_cross_CrossRenderer_shutdown(JNIEnv *env, jobject obj);

    void Java_org_chronotext_cross_CrossRenderer_resize(JNIEnv *env, jobject obj, jint width, jint height);
    void Java_org_chronotext_cross_CrossRenderer_draw(JNIEnv *env, jobject obj);
    
    void Java_org_chronotext_cross_CrossRenderer_addTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y);
    void Java_org_chronotext_cross_CrossRenderer_updateTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y);
    void Java_org_chronotext_cross_CrossRenderer_removeTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y);
    
    void Java_org_chronotext_cross_CrossRenderer_dispatchEvent(JNIEnv *env, jobject obj, jint eventId);
    void Java_org_chronotext_cross_CrossBridge_sendMessageToSketch(JNIEnv *env, jobject obj, jint what, jstring body);
}

// ---

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
}
