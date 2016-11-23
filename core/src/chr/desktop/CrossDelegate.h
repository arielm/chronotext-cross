#pragma once

#include "chr/cross/CrossDelegateBase.h"
#include "chr/cross/MouseEvent.h"
#include "chr/cross/KeyEvent.h"
#include "chr/cross/WheelEvent.h"
#include "chr/cross/Keyboard.h"

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
    int currentKeyCode = 0;

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

    static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void characterCallback(GLFWwindow *window, unsigned int codePoint);
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
  };
}
