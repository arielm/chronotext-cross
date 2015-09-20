/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cross/CrossDelegate.h"
#include "system/SystemManager.h"

namespace chr
{
    CrossSketch* createSketch();
    
    // ---

    CrossDelegate& delegate();
    SystemManager& systemManager();

    template <class T>
    inline T& checkedReference(T *instance)
    {
        assert(instance);
        return *instance;
    }
    
//    namespace os
//    {
//        /*
//         * RETURNS FALSE IF NOT INVOKED ON THE SKETCH-THREAD
//         */
//        bool isThreadSafe();
//
//        /*
//         * RETURNS FALSE IF THE FUNCTION CAN'T BE POSTED
//         *
//         * CAUSES:
//         *
//         * - IO-SERVICE IS NOT DEFINED
//         * - THE CONTEXT IS BEING SHUT-DOWN (TODO)
//         *
//         * SYNCHRONOUS POSTING ONLY:
//         * - NOT INVOKED ON THE SKETCH-THREAD
//         */
//        bool post(std::function<void()> &&fn, bool forceSync = false);
//    }
}
