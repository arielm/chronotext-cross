/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "system/SystemInfoBase.h"

#include <jni.h>

namespace chr
{
    namespace system
    {
        struct InitInfo
        {
            jobject androidContext = nullptr;
            jobject androidDisplay = nullptr;
            
            glm::vec2 displaySize {};
            float displayDensity = 0;
        };
        
        class Info : public InfoBase
        {
        public:
            int sdkVersion = 0;
            std::string model;
            std::string manufacturer;
        };
    }
}
