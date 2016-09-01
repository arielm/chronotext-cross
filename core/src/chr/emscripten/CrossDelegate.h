#pragma once

#include "chr/cross/CrossDelegateBase.h"
#include "chr/cross/MouseEvent.h"
#include "chr/cross/KeyEvent.h"
#include "chr/cross/WheelEvent.h"
#include "chr/cross/Keyboard.h"

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

  protected:
    int updateCount = 0;

    std::vector<MouseEvent> mouseEvents;
    std::vector<KeyEvent> keyEvents;
    std::vector<WheelEvent> wheelEvents;

    float mouseX;
    float mouseY;
    int mouseButton = -1;
    bool mousePressed = false;

    void processMouseEvents();
    void clearMouseEvents();

    void processKeyEvents();
    void clearKeyEvents();
    int convertKeyCode(int keyCode);

    void processWheelEvents();
    void clearWheelEvents();
    
    static void mainLoopCallback();

    static EM_BOOL resizeCallback(int eventType, const EmscriptenUiEvent *e, void *userData);
    static EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent *e, void *userData);
    static EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent *e, void *userData);
    static EM_BOOL wheelCallback(int eventType, const EmscriptenWheelEvent *wheelEvent, void *userData);
  };
}
