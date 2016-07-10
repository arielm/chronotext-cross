#include "Stub.h"

#include "android/CrossBridge.h"

using namespace std;
using namespace chr;

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
  return jni::onLoad(vm, reserved);
}

// ---

void Java_org_chronotext_cross_CrossBridge_init(JNIEnv *env, jobject obj, jobject bridge, jobject context, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity)
{
  return bridge::init(env, obj, bridge, context, display, displayWidth, displayHeight, displayDensity);
}

void Java_org_chronotext_cross_CrossBridge_uninit(JNIEnv *env, jobject obj)
{
  return bridge::uninit(env, obj);
}

void Java_org_chronotext_cross_CrossRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height)
{
  return bridge::setup(env, obj, width, height);
}

void Java_org_chronotext_cross_CrossRenderer_shutdown(JNIEnv *env, jobject obj)
{
  return bridge::shutdown(env, obj);
}

// ---

void Java_org_chronotext_cross_CrossRenderer_resize(JNIEnv *env, jobject obj, jint width, jint height)
{
  return bridge::resize(env, obj, width, height);
}

void Java_org_chronotext_cross_CrossRenderer_draw(JNIEnv *env, jobject obj)
{
  return bridge::draw(env, obj);
}

// ---

void Java_org_chronotext_cross_CrossRenderer_addTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
  return bridge::addTouch(env, obj, index, x, y);
}

void Java_org_chronotext_cross_CrossRenderer_updateTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
  return bridge::updateTouch(env, obj, index, x, y);
}

void Java_org_chronotext_cross_CrossRenderer_removeTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
  return bridge::removeTouch(env, obj, index, x, y);
}

// ---

void Java_org_chronotext_cross_CrossRenderer_dispatchEvent(JNIEnv *env, jobject obj, jint eventId)
{
  return bridge::dispatchEvent(env, obj, eventId);
}

void Java_org_chronotext_cross_CrossBridge_sendMessageToSketch(JNIEnv *env, jobject obj, jint what, jstring body)
{
  return bridge::sendMessageToSketch(env, obj, what, body);
}
