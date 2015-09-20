/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "desktop/system/SystemManager.h"
#include "cross/Context.h"

#include <boost/algorithm/string.hpp>

using namespace std;

namespace chr
{
    namespace system
    {
        void Manager::setup(const InitInfo &initInfo)
        {
            updateInfo();

            LOGI << "SYSTEM INFO: " << info << endl; // LOG: VERBOSE
        }

        // ---

        void Manager::updateInfo()
        {
            ManagerBase::updateInfo();
        }
        
        array<int, 3> Manager::getOsVersion()
        {
            return {0, 0, 0}; // FIXME
        }
        
        string Manager::getOsVersionString()
        {
            return "0.0.0"; // FIXME
        }
        
        string Manager::getDeviceString()
        {
            return "???"; // FIXME
        }
    }
}
