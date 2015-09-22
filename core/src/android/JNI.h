/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "Platform.h"
#include "Log.h"

#include <jni.h>
#include <android/asset_manager_jni.h>

extern "C"
{
    jint JNI_OnLoad(JavaVM *vm, void *reserved);
}

// ---

namespace chr
{
    namespace android
    {
        extern AAssetManager *assetManager;
        extern std::string internalDataPath;
        extern std::string externalDataPath;
        extern std::string apkPath;

        void initFileSystem(JNIEnv *env, jobject context);
    }

    namespace jni
    {
        extern JavaVM *vm;
        
        // ---
        
        JNIEnv* getEnv();
        
        std::string toString(jstring s);
        jstring toJString(const std::string &s);
    }
}
