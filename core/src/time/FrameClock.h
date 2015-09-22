/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "time/Clock.h"

namespace chr
{
    class FrameClock : public Clock
    {
    public:
        typedef std::shared_ptr<FrameClock> Ref;
        
        template<typename... T>
        static Ref create(T&&... args)
        {
            return Ref(new FrameClock(std::forward<T>(args)...));
        }
        
        double getTime() final;
        void setTime(double now) final;
        
        void update(bool immediately = true);
        void lock();
        void unlock();
        
    protected:
        double frameTime = 0;
        
        bool shouldSample = true;
        bool locked = false;
        
        FrameClock() = default;
        FrameClock(std::shared_ptr<TimeBase> timeBase, bool startOnConstruction = true);
    };
}
