/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <memory>

namespace chr
{
    class TimeBase : public std::enable_shared_from_this<TimeBase>
    {
    public:
        virtual ~TimeBase() {}
        virtual double getTime() = 0;
        
        std::shared_ptr<TimeBase> ref() { return shared_from_this(); }
    };
}
