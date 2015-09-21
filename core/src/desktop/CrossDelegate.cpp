/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "desktop/CrossDelegate.h"
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
            if (glfwInit())
            {
                glfwWindowHint(GLFW_SAMPLES, initInfo.windowInfo.aaSamples);
                glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

                window = glfwCreateWindow(initInfo.windowInfo.size.x, initInfo.windowInfo.size.y, "", NULL, NULL);

                if (window)
                {
                    glfwMakeContextCurrent(window);
                    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

                    // ---

                    intern::instance = this;
                    initialized_ = _init();
                }
            }
        }
        
        return initialized_;
    }
    
    void CrossDelegate::performUninit()
    {
        if (initialized_ && !setup_)
        {
            _uninit();
            window = nullptr;

            initialized_ = false;
            intern::instance = nullptr;
        }
    }
    
    void CrossDelegate::performSetup()
    {
        if (!setup_ && initialized_)
        {
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

    void CrossDelegate::run(int width, int height, int aaSamples)
    {
        initInfo.windowInfo = setupInfo.windowInfo = WindowInfo(width, height, aaSamples);

        performInit();
        performSetup();
        performResize(setupInfo.windowInfo.size);

        sketch->performStart(CrossSketch::START_REASON_VIEW_SHOWN);

        while (!glfwWindowShouldClose(window))
        {
            performUpdate();
            performDraw();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        sketch->performStop(CrossSketch::STOP_REASON_VIEW_HIDDEN);

        performShutdown();
        performUninit();
    }
}
