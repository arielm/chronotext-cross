#pragma once

#include "chr/cross/CrossDelegateBase.h"
#include "chr/cross/MouseEvent.h"
#include "chr/cross/TouchEvent.h"
#include "chr/cross/KeyEvent.h"
#include "chr/cross/WheelEvent.h"
#include "chr/cross/Keyboard.h"

#include <array>

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

    void run(int aaSamples = 0, int depthBits = 0);

    void enableAccelerometer( float updateFrequency = 30, float filterFactor = 0.1f) final;
    void disableAccelerometer() final;

    void measureCanvas();

  protected:
    static constexpr int FINGERS_CAPACITY = 10;

    enum
    {
      BROWSER_PLATFORM_UNDEFINED,
      BROWSER_PLATFORM_ANDROID,
      BROWSER_PLATFORM_IOS
    };

    int updateCount = 0;

    std::vector<MouseEvent> mouseEvents;
    std::vector<TouchEvent> touchEvents;
    std::vector<KeyEvent> keyEvents;
    std::vector<WheelEvent> wheelEvents;
    std::vector<AccelEvent> accelerationEvents;

    math::Rectf canvasBounds;

    float mouseX;
    float mouseY;
    int mouseButton = -1;
    bool mousePressed = false;

    std::array<uint64_t, FINGERS_CAPACITY> fingers;

    int browserPlatform = BROWSER_PLATFORM_UNDEFINED;

    void processMouseEvents();
    void clearMouseEvents();

    void processTouchEvents();
    void clearTouchEvents();
    int addFinger(uint64_t identifier);
    int updateFinger(uint64_t identifier);
    int removeFinger(uint64_t identifier);

    void processKeyEvents();
    void clearKeyEvents();
    int convertKeyCode(int keyCode);

    void processWheelEvents();
    void clearWheelEvents();

    void processAccelerationEvents();
    void clearAccelerationEvents();

    static void mainLoopCallback();

    static EM_BOOL resizeCallback(int eventType, const EmscriptenUiEvent *e, void *userData);
    static EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent *e, void *userData);
    static EM_BOOL touchCallback(int eventType, const EmscriptenTouchEvent *e, void *userData);
    static EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent *e, void *userData);
    static EM_BOOL wheelCallback(int eventType, const EmscriptenWheelEvent *wheelEvent, void *userData);
    static EM_BOOL deviceMotionCallback(int eventType, const EmscriptenDeviceMotionEvent *e, void *userData);
  };
}
