/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cross/CrossDelegateBase.h"
#include "ios/Touch.h"

namespace chr
{
    class CrossDelegate : public CrossDelegateBase
    {
    public:
        bool performInit();
        void performUninit();
        void performSetup(const WindowInfo &windowInfo);
        void performShutdown();
        
        void performResize(const glm::vec2 &size);
        void performUpdate();
        void performDraw();

       void touchesBegan(const std::vector<Touch> &touches);
       void touchesMoved(const std::vector<Touch> &touches);
       void touchesEnded(const std::vector<Touch> &touches);

        void sendMessageToBridge(int what, const std::string &body = "") final;
        void handleEvent(int eventId) final;

        void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f) final;
        void disableAccelerometer() final;
        void handleAcceleration(const glm::vec3 &acceleration);
        
//      ci::JsonTree jsonQuery(const char *methodName) final;
        
    protected:
        int updateCount = 0;
    };
}
