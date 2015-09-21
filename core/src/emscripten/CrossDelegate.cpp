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
            emscripten_set_canvas_size(initInfo.windowInfo.size.x, initInfo.windowInfo.size.y);

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
//        sketch->clock()->update(true);

        sketch->performUpdate();
        updateCount++;
    }

    void CrossDelegate::run(int width, int height, int aaSamples)
    {
        initInfo.windowInfo = setupInfo.windowInfo = WindowInfo(width, height, aaSamples);

        performInit();
        performSetup();
        performResize(setupInfo.windowInfo.size);

        sketch->performStart(CrossSketch::START_REASON_VIEW_SHOWN);

        emscripten_set_main_loop_arg(performDraw, sketch, 0, 1);

        sketch->performStop(CrossSketch::STOP_REASON_VIEW_HIDDEN);

        performShutdown();
        performUninit();
    }

    void CrossDelegate::performDraw(void *data)
    {
      reinterpret_cast<CrossSketch*>(data)->draw();
    }
}
