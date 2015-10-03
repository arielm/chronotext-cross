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
            emscripten_set_mousedown_callback(0, 0, 1, mouseCallback);
            emscripten_set_mouseup_callback(0, 0, 1, mouseCallback);
            emscripten_set_mousemove_callback(0, 0, 1, mouseCallback);

            // ---

            if (initInfo.windowInfo.size.x * initInfo.windowInfo.size.y == 0)
            {
                int innerWidth = EM_ASM_INT_V("return window.innerWidth");
                int innerHeight = EM_ASM_INT_V("return window.innerHeight");
                emscripten_set_canvas_size(innerWidth, innerHeight);

                setupInfo.windowInfo = WindowInfo(innerWidth, innerHeight, initInfo.windowInfo.aaSamples);
            }
            else
            {
                setupInfo.windowInfo = WindowInfo(initInfo.windowInfo.size.x, initInfo.windowInfo.size.y, initInfo.windowInfo.aaSamples);
                emscripten_set_canvas_size(initInfo.windowInfo.size.x, initInfo.windowInfo.size.y); 
            }

            EmscriptenWebGLContextAttributes attr;
            emscripten_webgl_init_context_attributes(&attr);

            attr.alpha = attr.depth = attr.stencil = attr.preserveDrawingBuffer = attr.preferLowPowerToHighPerformance = attr.failIfMajorPerformanceCaveat = 0;
            attr.enableExtensionsByDefault = 1;
            attr.antialias = (initInfo.windowInfo.aaSamples > 0) ? 1 : 0;
            attr.premultipliedAlpha = 0;
            attr.majorVersion = 1;
            attr.minorVersion = 0;

            EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context(0, &attr);
            emscripten_webgl_make_context_current(ctx);

            // ---

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
        initInfo.windowInfo = WindowInfo(width, height, aaSamples);

        performInit();
        performSetup();
        performResize(setupInfo.windowInfo.size);

        sketch->performStart(CrossSketch::START_REASON_VIEW_SHOWN);

        emscripten_set_main_loop(mainLoopCallback, 0, 1);

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

    void CrossDelegate::mainLoopCallback()
    {
        intern::instance->processMouseEvents();

        intern::instance->performUpdate();
        intern::instance->performDraw();

        intern::instance->clearMouseEvents();
    }

    EM_BOOL CrossDelegate::mouseCallback(int eventType, const EmscriptenMouseEvent *e, void *userData)
    {
        switch (eventType)
        {
            case EMSCRIPTEN_EVENT_MOUSEDOWN:
            {
                intern::instance->mouseEvents.emplace_back(intern::instance->mouseX, intern::instance->mouseY, e->button, MouseEvent::KIND_PRESSED);

                intern::instance->mouseButton = e->button;
                intern::instance->mousePressed = true;
                break;
            }

            case EMSCRIPTEN_EVENT_MOUSEUP:
            {
                intern::instance->mouseEvents.emplace_back(intern::instance->mouseX, intern::instance->mouseY, e->button, MouseEvent::KIND_RELEASED);

                intern::instance->mouseButton = e->button;
                intern::instance->mousePressed = false;
                break;
            }

            case EMSCRIPTEN_EVENT_MOUSEMOVE:
            {
                intern::instance->mouseEvents.emplace_back(e->clientX, e->clientY, intern::instance->mouseButton, intern::instance->mousePressed ? MouseEvent::KIND_DRAGGED : MouseEvent::KIND_MOVED);

                intern::instance->mouseX = e->clientX;
                intern::instance->mouseY = e->clientY;
                break;
            }
        }

        return 0;
    }
}
