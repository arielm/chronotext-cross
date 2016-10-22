#pragma once

#include "chr/cross/CrossSketch.h"
#include "chr/system/SystemManager.h"

//#include "cinder/Json.h"

#include <atomic>

namespace chr
{
  class CrossDelegateBase
  {
  public:
    virtual ~CrossDelegateBase() {}
    
    virtual void messageFromBridge(int what, const std::string &body = "") {}
    virtual void sendMessageToBridge(int what, const std::string &body = "") {}
    
    virtual void handleEvent(int eventId) {}
    virtual void performAction(int actionId) {}
    
    virtual void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f) {}
    virtual void disableAccelerometer() {}
    
//    virtual ci::JsonTree jsonQuery(const char *methodName) { return ci::JsonTree(); }
    
  protected:
    CrossSketch *sketch = nullptr;
    
    bool initialized_ = false;
    bool setup_ = false;

    system::InitInfo initInfo;
    system::SetupInfo setupInfo;
    
    AccelEvent::Filter accelFilter;

    virtual void sketchCreated(CrossSketch *sketch) {}
    virtual void sketchDestroyed(CrossSketch *sketch) {}
    
    bool _init();
    void _uninit();
    void _setup();
    void _shutdown();

    friend class CrossSketch;
  };
}
