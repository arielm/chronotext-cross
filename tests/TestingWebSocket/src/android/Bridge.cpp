#include "android/Bridge.h"

#include <vector>

using namespace std;

void Java_org_chronotext_TestingWebSocket_MainActivity_performInit(JNIEnv *env, jobject obj, jobject activity)
{
  chr::android::initFileSystem(env, activity);
}

jint Java_org_chronotext_TestingWebSocket_MainActivity_invokeMain(JNIEnv *env, jobject obj)
{
  vector<const char*> argv;
  argv.emplace_back(chr::android::apkPath.data());

  return main(argv.size(), argv.data());
}
