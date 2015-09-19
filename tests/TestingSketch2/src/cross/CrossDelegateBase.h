/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cross/CrossSketch.h"

#include <boost/asio.hpp>

//#include "cinder/Json.h"

#include <atomic>

namespace chr
{
    CrossSketch* createSketch();

    namespace system
    {
      struct SetupInfo
      {
          boost::asio::io_service *io_service;
          WindowInfo windowInfo;

          SetupInfo() = default;

          SetupInfo(boost::asio::io_service &io_service, const WindowInfo &windowInfo)
          :
          io_service(&io_service),
          windowInfo(windowInfo)
          {}
      };
    }

    // ---

    class CrossDelegateBase
    {
    public:
        static std::atomic<bool> LOG_VERBOSE;
        static std::atomic<bool> LOG_WARNING;

        virtual ~CrossDelegateBase() {}
        
        virtual void messageFromBridge(int what, const std::string &body = "");
        virtual void sendMessageToBridge(int what, const std::string &body = "") {}
        
        virtual void handleEvent(int eventId) {}
        virtual void performAction(int actionId) {}
        
        virtual void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f) {}
        virtual void disableAccelerometer() {}
        
//        virtual ci::JsonTree jsonQuery(const char *methodName) { return ci::JsonTree(); }
        
        /*
         * ci::app::KeyEvent IS NOT FULLY FUNCTIONAL ON MOBILE PLATFORMS
         *
         * HENCE THE FOLLOWING METHODS ALLOWING "FULL KEYBOARD
         * INTERACTION" ON THE DESKTOP WITHOUT USING MACROS
         */
//        static int getCode(const ci::app::KeyEvent &keyEvent);
//        static bool isShiftDown(const ci::app::KeyEvent &keyEvent);
//        static bool isAltDown(const ci::app::KeyEvent &keyEvent);
//        static bool isAccelDown(const ci::app::KeyEvent &keyEvent);
        
    protected:
        CrossSketch *sketch = nullptr;
        
        bool initialized_ = false;
        bool setup_ = false;

//        system::InitInfo initInfo;
        system::SetupInfo setupInfo;
        
//        AccelEvent::Filter accelFilter;

        virtual void sketchCreated(CrossSketch *sketch) {}
        virtual void sketchDestroyed(CrossSketch *sketch) {}
        
        bool _init();
        void _uninit();
        void _setup();
        void _shutdown();
    };
}
