#pragma once

#include "cross/CrossDelegateBase.h"
#include "android/CrossBridge.h"

#include <boost/asio.hpp>

#include <android/sensor.h>

namespace chr
{
  class CrossDelegate : public CrossDelegateBase
  {
  public:
    /*
     * INVOKED ON THE MAIN-THREAD
     *
     * RENDERER'S THREAD IS NOT AVAILABLE (EITHER NOT CREATED YET, OR ALREADY DESTROYED)
     */
    bool performInit(JNIEnv *env, jobject androidContext, jobject androidDisplay, const glm::vec2 &displaySize, float displayDensity);
    void performUninit(JNIEnv *env);
    
    /*
     * INVOKED ON THE RENDERER'S THREAD
     *
     * GL-CONTEXT IS VALID
     */
    void performSetup(JNIEnv *env, const glm::vec2 &size);
    void performShutdown(JNIEnv *env);
    
    void performResize(const glm::vec2 &size);
    void performUpdate();
    void performDraw();

    void addTouch(int index, float x, float y);
    void updateTouch(int index, float x, float y);
    void removeTouch(int index, float x, float y);
    
    void messageFromBridge(int what, const std::string &body = "") final;
    void sendMessageToBridge(int what, const std::string &body = "") final;
    
    void handleEvent(int eventId) final;
    void performAction(int actionId) final;

    void enableAccelerometer( float updateFrequency = 30, float filterFactor = 0.1f) final;
    void disableAccelerometer() final;

  protected:
    int updateCount = 0;

    std::shared_ptr<boost::asio::io_service> io;
    std::shared_ptr<boost::asio::io_service::work> ioWork;
    
    ASensorManager *sensorManager;
    const ASensor *accelerometerSensor;
    ASensorEventQueue *sensorEventQueue;
    
    void startIOService();
    void stopIOService();
    
    void createSensorEventQueue();
    void destroySensorEventQueue();
    void pollSensorEvents();

    void handleAcceleration(ASensorEvent event);
    int getDisplayRotation();
  };
}
