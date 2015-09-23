/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "system/DisplayHelperBase.h"
#include "system/SystemInfo.h"

namespace chr
{
    namespace display
    {
        class Helper : public HelperBase
        {
        public:
            static void setup(const system::InitInfo &initInfo);
            static void shutdown();
        };
    }
}
