/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "FrameClock.h"

using namespace std;

namespace chr
{
    FrameClock::FrameClock(shared_ptr<TimeBase> timeBase, bool startOnConstruction)
    :
    Clock(timeBase, startOnConstruction)
    {}
    
    double FrameClock::getTime()
    {
        if (shouldSample && !locked)
        {
            frameTime = Clock::getTime();
            shouldSample = false;
        }
        
        return frameTime;
    }
    
    void FrameClock::setTime(double now)
    {
        Clock::setTime(now);
        frameTime = now;
    }
    
    void FrameClock::update(bool immediately)
    {
        locked = false;
        
        if (immediately)
        {
            frameTime = Clock::getTime();
        }
        else
        {
            shouldSample = true;
        }
    }
    
    void FrameClock::lock()
    {
        locked = true;
    }
    
    void FrameClock::unlock()
    {
        locked = false;
    }
}
