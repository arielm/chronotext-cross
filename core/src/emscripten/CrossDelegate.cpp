/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "emscripten/CrossDelegate.h"
#include "cross/Context.h"

using namespace std;

namespace chr
{
    namespace intern
    {
        CrossDelegate *instance = nullptr;
    }

    CrossDelegate& delegate()
    {
        return checkedReference(intern::instance);
    }
    
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

        sketch->performUpdate();
        updateCount++;
    }
    
    void CrossDelegate::performDraw()
    {
        if (updateCount == 0)
        {
            performUpdate(); // HANDLING CASES WHERE draw() IS INVOKED BEFORE update()
        }

        sketch->draw();
    }
}
