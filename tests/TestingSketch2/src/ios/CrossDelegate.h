/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

//#include "cinder/Cinder.h"

#if !defined(CINDER_COCOA_TOUCH)
#   error UNSUPPORTED PLATFORM
#endif

// ---

#include "cross/CrossDelegateBase.h"

//#include "cinder/app/TouchEvent.h"

#include <boost/asio.hpp>

namespace chr
{
    class CinderDelegate : public CinderDelegateBase
    {
    public:
        bool performInit();
        void performUninit();
        void performSetup(const WindowInfo &windowInfo);
        void performShutdown();
        
        void performResize(const ci::Vec2i &size);
        void performUpdate();
        void performDraw();

        void touchesBegan(ci::app::TouchEvent event);
        void touchesMoved(ci::app::TouchEvent event);
        void touchesEnded(ci::app::TouchEvent event);

        void sendMessageToBridge(int what, const std::string &body = "") final;
        void handleEvent(int eventId) final;

        void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f) final;
        void disableAccelerometer() final;
        void handleAcceleration(const ci::Vec3f &acceleration);
        
//      ci::JsonTree jsonQuery(const char *methodName) final;
        
    protected:
        int updateCount = 0;

        std::shared_ptr<boost::asio::io_service> io;
        std::shared_ptr<boost::asio::io_service::work> ioWork;
        
        void startIOService();
        void stopIOService();
    };
}
