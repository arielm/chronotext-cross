/*
 * THIS MUST BE PART OF THE SKETCH'S SHARED-LIB
 */

#pragma once

#include <jni.h>

extern "C"
{
  jint JNI_OnLoad(JavaVM *vm, void *reserved);

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
