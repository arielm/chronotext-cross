#pragma once

#include "chr/android/JNI.h"

extern "C"
{
  void Java_org_chronotext_TestingFileSystem3_MainActivity_performInit(JNIEnv *env, jobject obj, jobject activity);
  jint Java_org_chronotext_TestingFileSystem3_MainActivity_invokeMain(JNIEnv *env, jobject obj);
}

extern int main(int argc, const char *argv[]);
