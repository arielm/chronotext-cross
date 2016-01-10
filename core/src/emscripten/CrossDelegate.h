/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cross/CrossDelegateBase.h"
#include "cross/MouseEvent.h"

#include <emscripten.h>

namespace chr
{
    class CrossDelegate : public CrossDelegateBase
    {
    public:
        bool performInit();
        void performUninit();
        void performSetup();
        void performShutdown();
        
        void performResize(const glm::vec2 &size);
        void performUpdate();
        void performDraw();

        void run(int width, int height, int aaSamples = 0, int depthBits = 0);

    protected:
        int updateCount = 0;

        std::vector<MouseEvent> mouseEvents;
        float mouseX;
        float mouseY;
        int mouseButton = -1;
        bool mousePressed = false;

        void processMouseEvents();
        void clearMouseEvents();
        
        static void mainLoopCallback();
        static EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent *e, void *userData);
    };
}
