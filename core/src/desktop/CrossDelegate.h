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

        void run(int width, int height, int aaSamples = 0);

    protected:
        int updateCount = 0;

        std::vector<MouseEvent> mouseEvents;
        float mouseX;
        float mouseY;
        int mouseButton = -1;
        bool mousePressed = false;

        void processMouseEvents();
        void clearMouseEvents();

        static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    };
}
