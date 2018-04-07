#pragma once

#include "chr/cross/CrossDelegateBase.h"
#include "chr/cross/MouseEvent.h"
#include "chr/cross/KeyEvent.h"
#include "chr/cross/WheelEvent.h"
#include "chr/cross/Keyboard.h"

typedef BOOL (__stdcall *PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC hdc, const int*, const FLOAT*, UINT, int*, UINT*); // XXX
typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);

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

    void showCursor() final;
    void hideCursor() final;

    void run(int width, int height, int aaSamples = 0, int depthBits = 0);

  protected:
    int updateCount = 0;

    std::vector<MouseEvent> mouseEvents;
    std::vector<KeyEvent> keyEvents;
    std::vector<WheelEvent> wheelEvents;

    bool windowShouldClose = false;
    int mouseX, mouseY;
    int mouseButton = -1;

    HWND wnd;
    HDC dc;
    HGLRC rc;
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
    PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = nullptr;

    void processMouseEvents();
    void clearMouseEvents();

    void processKeyEvents();
    void clearKeyEvents();
    int convertKeyCode(int keyCode);

    void processWheelEvents();
    void clearWheelEvents();

    bool createWindow(int width, int height, int aaSamples = 0, int depthBits = 0);
    void closeWindow();
    void getWglFunctionPointers();

    void handleKeyEvent(KeyEvent::Kind kind, int keyCode);
    static int getKeyModifiers();

    friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
  };
}
