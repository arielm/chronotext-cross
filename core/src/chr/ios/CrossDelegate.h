#pragma once

#include "chr/cross/CrossDelegateBase.h"
#include "chr/cross/TouchEvent.h"

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

    void touchesBegan(const std::vector<TouchEvent> &touches);
    void touchesMoved(const std::vector<TouchEvent> &touches);
    void touchesEnded(const std::vector<TouchEvent> &touches);

    void sendMessageToBridge(int what, const std::string &body = "") final;
    void handleEvent(int eventId) final;

    void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f) final;
    void disableAccelerometer() final;
    void handleAcceleration(const glm::vec3 &acceleration);
    
//    ci::JsonTree jsonQuery(const char *methodName) final;
    
  protected:
    int updateCount = 0;
  };
}
