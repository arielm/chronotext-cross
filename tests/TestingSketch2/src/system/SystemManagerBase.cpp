/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "system/SystemManagerBase.h"
#include "cross/Context.h"

using namespace std;

namespace chr
{
    namespace system
    {
        const string& platformName()
        {
            static const string PLATFORM_NAMES[5] = { "OSX", "iOS", "Android", "Emscripten", "Windows" };
            return PLATFORM_NAMES[CHR_PLATFORM];
        }

        // ---
        
        const Info& ManagerBase::getInfo() const
        {
            return info;
        }
        
        void ManagerBase::updateInfo()
        {
            info.platformString = platformName();
            
            info.osVersion = getOsVersion();
            info.osVersionString = getOsVersionString();
            
            info.deviceString = getDeviceString();
        }
    }
    
    SystemInfo getSystemInfo()
    {
        return systemManager().getInfo();
    }
}
