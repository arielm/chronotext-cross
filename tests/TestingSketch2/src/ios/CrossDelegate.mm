/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "ios/CrossDelegate.h"

#import "ios/CrossBridge.h"

using namespace std;

namespace chr
{
    namespace intern
    {
        CrossDelegate *instance = nullptr;
    }

//    CrossDelegate& delegate()
//    {
//        return checkedReference(intern::instance);
//    }
    
    // ---
    
    bool CrossDelegate::performInit()
    {
        if (!initialized_)
        {
            intern::instance = this;
            initialized_ = _init();
        }
        
        return initialized_;
    }
    
    void CrossDelegate::performUninit()
    {
        if (initialized_ && !setup_)
        {
            _uninit();

            initialized_ = false;
            intern::instance = nullptr;
        }
    }
    
    void CrossDelegate::performSetup(const WindowInfo &windowInfo)
    {
        if (!setup_ && initialized_)
        {
            startIOService();

            setupInfo.io_service = io.get();
            setupInfo.windowInfo = windowInfo;

            _setup();

            // ---

            setup_ = true;
        }
    }
    
    void CrossDelegate::performShutdown()
    {
        if (setup_)
        {
            /*
             * TODO:
             *
             * - HANDLE PROPERLY THE SHUTING-DOWN OF "UNDERGOING" TASKS
             * - SEE RELATED TODOS IN CinderDelegateBase AND TaskManager
             */
            _shutdown();

            stopIOService();

            // ---

            setup_ = false;
        }
    }
    
    void CrossDelegate::performResize(const glm::vec2 &size)
    {
        setupInfo.windowInfo.size = size;
        sketch->performResize(size);
    }
    
    void CrossDelegate::performUpdate()
    {
        /*
         * SHOULD TAKE PLACE BEFORE IO-SERVICE-POLLING
         *
         * SUBSEQUENT CALLS TO FrameClock::getTime() DURING THE FRAME WILL RETURN THE SAME TIME-SAMPLE
         */
//        sketch->clock()->update(true);

        io->poll();

        sketch->performUpdate();
        updateCount++;
    }
    
    void CrossDelegate::performDraw()
    {
        if (updateCount == 0)
        {
            performUpdate(); // HANDLING CASES WHERE draw() IS INVOKED BEFORE update()
        }

//        sketch->draw();
    }
    
#pragma mark ---------------------------------------- SKETCH <-> BRIDGE COMMUNICATION ----------------------------------------
    
    void CrossDelegate::sendMessageToBridge(int what, const string &body)
    {
//        [system::bridge dispatchMessage:what body:[NSString stringWithUTF8String:body.data()]];
    }
    
    void CrossDelegate::handleEvent(int eventId)
    {
        switch (eventId)
        {
            case CrossSketch::EVENT_RESUMED:
                sketch->performStart(CrossSketch::START_REASON_APP_RESUMED);
                return;

            case CrossSketch::EVENT_SHOWN:
                sketch->performStart(CrossSketch::START_REASON_VIEW_SHOWN);
                return;

            case CrossSketch::EVENT_PAUSED:
                sketch->performStop(CrossSketch::STOP_REASON_APP_PAUSED);
                return;

            case CrossSketch::EVENT_HIDDEN:
                sketch->performStop(CrossSketch::STOP_REASON_VIEW_HIDDEN);
                return;
        }

        sketch->event(eventId);
    }
    
    // ---
    
    /*
     * TODO:
     *
     * 1) FINALIZE THREAD-SAFETY POLICY
     * 2) HANDLE POTENTIAL OBJECTIVE-C EXCEPTIONS WHILE "PERFORMING SELECTOR"
     */
    
//    JsonTree CrossDelegate::jsonQuery(const char *methodName)
//    {
//        SEL selector = NSSelectorFromString([NSString stringWithUTF8String:methodName]);
//
//        if ([system::bridge respondsToSelector:selector])
//        {
//            const string &query = [[system::bridge performSelector:selector] UTF8String];
//
//            if (!query.empty())
//            {
//                try
//                {
//                    return JsonTree(query);
//                }
//                catch (exception &e)
//                {
//                    LOGI_IF(LOG_WARNING)  << "JSON-QUERY FAILED | REASON: " << e.what() << endl;
//                }
//            }
//        }
//
//        return JsonTree();
//    }
    
#pragma mark ---------------------------------------- IO-SERVICE ----------------------------------------
    
    void CrossDelegate::startIOService()
    {
        if (!io)
        {
            io = make_shared<boost::asio::io_service>();
            ioWork = make_shared<boost::asio::io_service::work>(*io);
        }
    }
    
    void CrossDelegate::stopIOService()
    {
        if (io)
        {
            io->stop();

            ioWork.reset();
            io.reset();
        }
    }
    
#pragma mark ---------------------------------------- TOUCH ----------------------------------------
    
//    void CrossDelegate::touchesBegan(TouchEvent event)
//    {
//        for (auto &touch : event.getTouches())
//        {
//            sketch->addTouch(touch.getId() - 1, touch.getX(), touch.getY());
//        }
//    }
//
//    void CrossDelegate::touchesMoved(TouchEvent event)
//    {
//        for (auto &touch : event.getTouches())
//        {
//            sketch->updateTouch(touch.getId() - 1, touch.getX(), touch.getY());
//        }
//    }
//
//    void CrossDelegate::touchesEnded(TouchEvent event)
//    {
//        for (auto &touch : event.getTouches())
//        {
//            sketch->removeTouch(touch.getId() - 1, touch.getX(), touch.getY());
//        }
//    }
    
#pragma mark ---------------------------------------- KEYBOARD ----------------------------------------
    
//    int CrossDelegateBase::getCode(const KeyEvent &keyEvent)
//    {
//        return 0;
//    }
//
//    bool CrossDelegateBase::isShiftDown(const KeyEvent &keyEvent)
//    {
//        return false;
//    }
//
//    bool CrossDelegateBase::isAltDown(const KeyEvent &keyEvent)
//    {
//        return false;
//    }
//
//    bool CrossDelegateBase::isAccelDown(const KeyEvent &keyEvent)
//    {
//        return false;
//    }
    
#pragma mark ---------------------------------------- ACCELEROMETER ----------------------------------------
    
    /*
     * TODO: THE TIME HAS COME TO RELY ON THE CoreMotion FRAMEWORK
     */
    
    void CrossDelegate::enableAccelerometer(float updateFrequency, float filterFactor)
    {
//        accelFilter = AccelEvent::Filter(filterFactor);
//
//        if (updateFrequency <= 0)
//        {
//            updateFrequency = 30;
//        }
//
//        UIAccelerometer.sharedAccelerometer.updateInterval = 1 / updateFrequency;
//        UIAccelerometer.sharedAccelerometer.delegate = system::bridge;
    }
    
    void CrossDelegate::disableAccelerometer()
    {
        UIAccelerometer.sharedAccelerometer.delegate = nil;
    }
    
    void CrossDelegate::handleAcceleration(const glm::vec3 &acceleration)
    {
//        sketch->accelerated(accelFilter.process(acceleration));
    }
}
