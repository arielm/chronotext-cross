#include "chr/android/CrossBridge.h"
#include "chr/cross/Context.h"

using namespace std;

namespace chr
{
  namespace jni
  {
    jobject bridge = nullptr;

    CrossDelegate *crossDelegate = nullptr;

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

  namespace bridge
  {
    void init(JNIEnv *env, jobject obj, jobject bridge, jobject context, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity)
    {
      android::initFileSystem(env, context);

      // ---

      jni::bridge = env->NewGlobalRef(bridge);

      jni::crossDelegate = new CrossDelegate();
      jni::crossDelegate->performInit(env, context, display, glm::vec2(displayWidth, displayHeight), displayDensity);
    }

    void uninit(JNIEnv *env, jobject obj)
    {
      jni::crossDelegate->performUninit(env);
      delete jni::crossDelegate;

      env->DeleteGlobalRef(jni::bridge);
      jni::bridge = nullptr;
    }

    void setup(JNIEnv *env, jobject obj, jint width, jint height)
    {
      jni::crossDelegate->performSetup(env, glm::vec2(width, height));
    }

    void shutdown(JNIEnv *env, jobject obj)
    {
      jni::crossDelegate->performShutdown(env);
    }

    // ---

    void resize(JNIEnv *env, jobject obj, jint width, jint height)
    {
      jni::crossDelegate->performResize(glm::vec2(width, height));
    }

    void draw(JNIEnv *env, jobject obj)
    {
      jni::crossDelegate->performUpdate();
      jni::crossDelegate->performDraw();
    }

    // ---

    void addTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
    {
      jni::crossDelegate->addTouch(index, x, y);
    }

    void updateTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
    {
      jni::crossDelegate->updateTouch(index, x, y);
    }

    void removeTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
    {
      jni::crossDelegate->removeTouch(index, x, y);
    }

    // ---

    void dispatchEvent(JNIEnv *env, jobject obj, jint eventId)
    {
      jni::crossDelegate->handleEvent(eventId);
    }

    void sendMessageToSketch(JNIEnv *env, jobject obj, jint what, jstring body)
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
  }
}
