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

                initInfo.window = glfwCreateWindow(initInfo.windowInfo.size.x, initInfo.windowInfo.size.y, "", NULL, NULL);

                if (initInfo.window)
                {
                    glfwSetCursorPosCallback(initInfo.window, cursorPosCallback);
                    glfwSetMouseButtonCallback(initInfo.window, mouseButtonCallback);
                    glfwSetKeyCallback(initInfo.window, keyCallback);

                    glfwMakeContextCurrent(initInfo.window);
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
         * SUBSEQUENT CALLS TO FrameClock::getTime() DURING THE FRAME WILL RETURN THE SAME TIME-SAMPLE
         */
        sketch->clock()->update(true);

        sketch->performUpdate();
        updateCount++;
    }
    
    void CrossDelegate::performDraw()
    {
        sketch->draw();
    }

    void CrossDelegate::run(int width, int height, int aaSamples)
    {
        initInfo.windowInfo = setupInfo.windowInfo = WindowInfo(width, height, aaSamples);

        performInit();
        performSetup();
        performResize(setupInfo.windowInfo.size);

        sketch->performStart(CrossSketch::START_REASON_VIEW_SHOWN);

        while (!glfwWindowShouldClose(initInfo.window))
        {
            intern::instance->processMouseEvents();
            performUpdate();
            performDraw();

            glfwSwapBuffers(initInfo.window);

            intern::instance->clearMouseEvents();
            glfwPollEvents();
        }

        sketch->performStop(CrossSketch::STOP_REASON_VIEW_HIDDEN);

        performShutdown();
        performUninit();
    }

    void CrossDelegate::processMouseEvents()
    {
        for (auto &event : mouseEvents)
        {
            switch (event.kind)
            {
                case MouseEvent::KIND_PRESSED:
                    sketch->addTouch(event.button, event.x, event.y);
                    break;

                case MouseEvent::KIND_DRAGGED:
                    sketch->updateTouch(event.button, event.x, event.y);
                    break;

                case MouseEvent::KIND_RELEASED:
                    sketch->removeTouch(event.button, event.x, event.y);
                    break;

                default:
                    break;
            }
        }
    }

    void CrossDelegate::clearMouseEvents()
    {
        mouseEvents.clear();
    }

    void CrossDelegate::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        intern::instance->mouseEvents.emplace_back(xpos, ypos, intern::instance->mouseButton, intern::instance->mousePressed ? MouseEvent::KIND_DRAGGED : MouseEvent::KIND_MOVED);

        intern::instance->mouseX = xpos;
        intern::instance->mouseY = ypos;
    }

    void CrossDelegate::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        intern::instance->mouseEvents.emplace_back(intern::instance->mouseX, intern::instance->mouseY, button, (action == GLFW_PRESS) ? MouseEvent::KIND_PRESSED : MouseEvent::KIND_RELEASED);

        intern::instance->mouseButton = button;
        intern::instance->mousePressed = (action == GLFW_PRESS);
    }

    void CrossDelegate::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, true);
        }
    }
}
