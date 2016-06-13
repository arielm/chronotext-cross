#pragma once

#include "cross/CrossDelegateBase.h"
#include "cross/MouseEvent.h"
#include "cross/KeyEvent.h"
#include "cross/Keyboard.h"

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
    std::vector<KeyEvent> keyEvents;

    float mouseX;
    float mouseY;
    int mouseButton = -1;
    bool mousePressed = false;

    void processMouseEvents();
    void clearMouseEvents();

    void processKeyEvents();
    void clearKeyEvents();
    int convertKeyCode(int keyCode);

    static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void characterCallback(GLFWwindow *window, unsigned int codePoint);
  };
}
