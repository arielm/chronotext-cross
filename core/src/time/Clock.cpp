/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Clock.h"

using namespace std;

namespace chr
{
    Clock::Clock()
    :
    timeBase(DefaultTimeBase::create()),
    mst(0),
    rate(1),
    state(STOPPED)
    {}
    
    Clock::Clock(shared_ptr<TimeBase> timeBase, bool startOnConstruction)
    :
    timeBase(timeBase),
    mst(0),
    rate(1),
    state(STOPPED)
    {
        if (startOnConstruction)
        {
            start();
        }
    }
    
    void Clock::start()
    {
        tbst = timeBase->getTime();
        state = STARTED;
    }
    
    void Clock::stop()
    {
        if (state != STOPPED)
        {
            mst = getTime();
            state = STOPPED;
        }
    }
    
    double Clock::getTime()
    {
        return mst + ((state == STOPPED) ? 0 : (timeBase->getTime() - tbst) * rate);
    }
    
    void Clock::setTime(double now)
    {
        if (state == STOPPED)
        {
            mst = now;
        }
//      else
//      {
//          throw EXCEPTION(Clock, "CLOCK SHOULD BE STOPPED");
//      }
    }
    
    double Clock::getRate()
    {
        return rate;
    }
    
    void Clock::setRate(double factor)
    {
        if (state == STOPPED)
        {
            rate = factor;
        }
//      else
//      {
//          throw EXCEPTION(Clock, "CLOCK SHOULD BE STOPPED");
//      }
    }
    
    Clock::State Clock::getState()
    {
        return state;
    }
    
    void Clock::restart()
    {
        stop();
        setTime(0);
        start();
    }
    
    void Clock::reset()
    {
        stop();
        setTime(0);
    }
}
