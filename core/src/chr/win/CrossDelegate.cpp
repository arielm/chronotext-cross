#include "chr/win/CrossDelegate.h"
#include "chr/win/Keyboard.h"

using namespace std;

namespace chr
{
  namespace intern
  {
    CrossDelegate *instance = nullptr;
  }

  CrossDelegate& delegate()
  {
    return checkedReference(intern::instance);
  }

  // ---

  LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

  bool CrossDelegate::performInit()
  {
    if (!initialized_)
    {
      getWglFunctionPointers();

      if (createWindow(initInfo.windowInfo.size.x, initInfo.windowInfo.size.y, initInfo.windowInfo.aaSamples, initInfo.windowInfo.depthBits))
      {
        int width = initInfo.windowInfo.size.x;
        int height = initInfo.windowInfo.size.y;

        if (width * height == 0)
        {
          RECT desktopRect;
          GetWindowRect(GetDesktopWindow(), &desktopRect);
          width = desktopRect.right;
          height = desktopRect.bottom;
        }

        /*
         * FIXME: setupInfo.windowInfo.aaSamples SHOULD REFLECT THE VALUE EFFECTIVELY SET DURING createWindow()
         */
        setupInfo.windowInfo = WindowInfo(width, height, initInfo.windowInfo.aaSamples, initInfo.windowInfo.depthBits);

        // ---

        intern::instance = this;
        initialized_ = _init();
      }
      else
      {
        closeWindow();
      }
    }

    return initialized_;
  }

  void CrossDelegate::performUninit()
  {
    if (initialized_ && !setup_)
    {
      _uninit();

      initialized_ = false;
      intern::instance = nullptr;
    }
  }

  void CrossDelegate::performSetup()
  {
    if (!setup_ && initialized_)
    {
      _setup();

      // ---

      setup_ = true;
    }
  }

  void CrossDelegate::performShutdown()
  {
    if (setup_)
    {
      _shutdown();

      // ---

      setup_ = false;
    }
  }

  void CrossDelegate::performResize(const glm::vec2 &size)
  {
    setupInfo.windowInfo.size = size;
    sketch->performResize(size);
  }

  void CrossDelegate::performUpdate()
  {
    /*
     * SUBSEQUENT CALLS TO FrameClock::getTime() DURING THE FRAME WILL RETURN THE SAME TIME-SAMPLE
     */
    sketch->clock()->update(true);

    sketch->performUpdate();
    updateCount++;
  }

  void CrossDelegate::performDraw()
  {
    sketch->draw();
    SwapBuffers(dc);
  }

  void CrossDelegate::run(int width, int height, int aaSamples, int depthBits)
  {
    initInfo.windowInfo = WindowInfo(width, height, aaSamples, depthBits);

    performInit(); // TODO: HANDLE FAILURES
    performSetup();
    performResize(setupInfo.windowInfo.size);

    sketch->performStart(CrossSketch::START_REASON_VIEW_SHOWN);

    while (!windowShouldClose)
    {
      intern::instance->processMouseEvents();
      intern::instance->processKeyEvents();
      intern::instance->processWheelEvents();

      intern::instance->clearMouseEvents();
      intern::instance->clearKeyEvents();
      intern::instance->clearWheelEvents();

      performUpdate();

      RedrawWindow(wnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

      MSG msg;
      while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }

    sketch->performStop(CrossSketch::STOP_REASON_VIEW_HIDDEN);

    performShutdown();
    performUninit();
  }

  void CrossDelegate::processMouseEvents()
  {
    for (const auto &event : mouseEvents)
    {
      switch (event.kind)
      {
        case MouseEvent::KIND_MOVED:
          sketch->mouseMoved(event.x, event.y);
          break;

        case MouseEvent::KIND_DRAGGED:
          sketch->mouseDragged(event.button, event.x, event.y);
          sketch->updateTouch(event.button, event.x, event.y);
          break;

        case MouseEvent::KIND_PRESSED:
          sketch->mousePressed(event.button, event.x, event.y);
          sketch->addTouch(event.button, event.x, event.y);
          break;

        case MouseEvent::KIND_RELEASED:
          sketch->mouseReleased(event.button, event.x, event.y);
          sketch->removeTouch(event.button, event.x, event.y);
          break;

        default:
          break;
      }
    }
  }

  void CrossDelegate::clearMouseEvents()
  {
    mouseEvents.clear();
  }

  void CrossDelegate::processKeyEvents()
  {
    for (const auto &event : keyEvents)
    {
      switch (event.kind)
      {
        case KeyEvent::KIND_PRESSED:
          sketch->keyPressed(event.codePoint);
          break;

        case KeyEvent::KIND_DOWN:
          sketch->keyDown(event.keyCode, event.modifiers);
          break;

        case KeyEvent::KIND_UP:
          sketch->keyUp(event.keyCode, event.modifiers);
          break;

        default:
          break;
      }
    }
  }

  void CrossDelegate::clearKeyEvents()
  {
    keyEvents.clear();
  }

  int CrossDelegate::convertKeyCode(int keyCode)
  {
    auto found = KEYMAP.find(keyCode);

    if (found != KEYMAP.end())
    {
      return found->second;
    }

    return keyCode;
  }

  void CrossDelegate::processWheelEvents()
  {
    for (const auto &event : wheelEvents)
    {
      sketch->wheelUpdated(event.offset);
    }
  }

  void CrossDelegate::clearWheelEvents()
  {
    wheelEvents.clear();
  }

  bool CrossDelegate::createWindow(int width, int height, int aaSamples, int depthBits)
  {
    WNDCLASS wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Redraw On Size, And Own DC For Window.
    wc.lpfnWndProc   = WndProc;                            // WndProc Handles Messages
    wc.cbClsExtra    = 0;                                  // No Extra Window Data
    wc.cbWndExtra    = 0;                                  // No Extra Window Data
    wc.hInstance     = GetModuleHandle(NULL);              // Set The Instance
    wc.hIcon         = LoadIcon( NULL, IDI_WINLOGO );      // Load The Default Icon
    wc.hCursor       = LoadCursor( NULL, IDC_ARROW );      // Load The Arrow Pointer
    wc.hbrBackground = NULL;                               // No Background Required For GL
    wc.lpszMenuName  = NULL;                               // We Don't Want A Menu
    wc.lpszClassName = L"WindowClass";

    if (!RegisterClass(&wc))
    {
      LOGE << "UNABLE TO REGISTER WINDOW CLASS" << endl;
      return false;
    }

    // ---

    RECT windowRect, desktopRect;
    DWORD windowExStyle, windowStyle;

    HWND desktopWindow = GetDesktopWindow();
    GetWindowRect(desktopWindow, &desktopRect);

    if (width * height == 0)
    {
      windowRect.left = 0;
      windowRect.right = desktopRect.right;
      windowRect.top = 0;
      windowRect.bottom = desktopRect.bottom;

      windowExStyle = WS_EX_APPWINDOW;
      windowStyle = WS_POPUP;
    }
    else
    {
      auto x = (desktopRect.right - width) / 2;
      auto y = (desktopRect.bottom - height) / 2;

      windowRect.left = x;
      windowRect.right = x + width;
      windowRect.top = y;
      windowRect.bottom = y + height;

      windowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
      windowStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX;
    }

    AdjustWindowRectEx(&windowRect, windowStyle, FALSE, windowExStyle);

    wnd = CreateWindowEx(windowExStyle,   // Extended Style For The Window
      L"WindowClass",                     // Window Class Name
      L"",                                // Window Title
      windowStyle,                        // Required Window Style
      windowRect.left, windowRect.top,
      windowRect.right - windowRect.left,
      windowRect.bottom - windowRect.top,
      NULL,                               // No Parent Window
      NULL,                               // No Menu
      GetModuleHandle(NULL),
      reinterpret_cast<LPVOID>(this));

    if (!wnd)
    {
      LOGE << "WINDOW CREATION FAILED" << endl;
      return false;
    }

    dc = GetDC(wnd);

    if (!dc)
    {
      LOGE << "DRAWING-CONTEXT CREATION FAILED" << endl;
      return false;
    }

    // ----

    int pixelFormat;
    bool aaInitialized = false;

    if ((aaSamples > 0) && wglChoosePixelFormatARB)
    {
      unsigned int numFormats;
      float fAttributes[] = {0, 0};

      int attributes[] =
      {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 24,
        WGL_DEPTH_BITS_ARB, depthBits,
        WGL_STENCIL_BITS_ARB, 8, // XXX
        WGL_SAMPLE_BUFFERS_ARB, 1,
        WGL_SAMPLES_ARB, 4, // XXX
        0
      };

      if (wglChoosePixelFormatARB(dc, attributes, fAttributes, 1, &pixelFormat, &numFormats))
      {
        aaInitialized = true;
      }
      else
      {
        LOGE << "INVALID ANTI-ALIASED PIXEL FORMAT" << endl;
      }
    }

    PIXELFORMATDESCRIPTOR pfd;

    if (!aaInitialized)
    {
      pfd =
      {
        sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
        1,                             // Version Number
        PFD_DRAW_TO_WINDOW |           // Format Must Support Window
          PFD_SUPPORT_OPENGL |         // Format Must Support OpenGL
          PFD_DOUBLEBUFFER,            // Must Support Double Buffering
        PFD_TYPE_RGBA,                 // Request An RGBA Format
        32,                            // Select Our Color Depth
        0, 0, 0, 0, 0, 0,              // Color Bits Ignored
        0,                             // No Alpha Buffer
        0,                             // Shift Bit Ignored
        0,                             // No Accumulation Buffer
        0, 0, 0, 0,                    // Accumulation Bits Ignored
        depthBits,                     // depth bits
        8,                             // stencil bits
        0,                             // No Auxiliary Buffer
        PFD_MAIN_PLANE,                // Main Drawing Layer
        0,                             // Reserved
        0, 0, 0                        // Layer Masks Ignored
      };

      pixelFormat = ChoosePixelFormat(dc, &pfd);

      if (pixelFormat == 0)
      {
        LOGE << "NO COMPATIBLE PIXEL-FORMAT FOUND" << endl;
        return false;
      }
    }

    if (!SetPixelFormat(dc, pixelFormat, &pfd))
    {
      LOGE << "UNABLE TO SET PIXEL-FORMAT" << endl;
      return false;
    }

    if (!(rc = wglCreateContext(dc)))
    {
      LOGE << "UNABLE TO GET RENDERING CONTEXT" << endl;
      return false;
    }

    if (!wglMakeCurrent(dc, rc))
    {
      LOGE << "CAN'T ACTIVATE RENDERING CONTEXT" << endl;
      return false;
    }

    // ---

    if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
    {
      LOGE << "glload FAILED" << endl;
      return false;
    }

    // ---

    ShowWindow(wnd, SW_SHOW);
    SetForegroundWindow(wnd);
    SetFocus(wnd);

    if (wglSwapIntervalEXT)
    {
      wglSwapIntervalEXT(1);
    }

    return true;
  }

  void CrossDelegate::closeWindow()
  {
    if (rc)
    {
      wglMakeCurrent(NULL, NULL);
      wglDeleteContext(rc);
    }

    if (wnd)
    {
      if (dc) ReleaseDC(wnd, dc);

      DestroyWindow(wnd);
      UnregisterClass(L"WindowClass", GetModuleHandle(NULL));
    }

    rc = NULL;
    dc = NULL;
    wnd = NULL;
    windowShouldClose = true;
  }

  void CrossDelegate::getWglFunctionPointers()
  {
    static PIXELFORMATDESCRIPTOR pfd =
    {
      sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
      1,                             // Version Number
      PFD_DRAW_TO_WINDOW |           // Format Must Support Window
        PFD_SUPPORT_OPENGL |         // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,            // Must Support Double Buffering
      PFD_TYPE_RGBA,                 // Request An RGBA Format
      32,                            // Select Our Color Depth
      0, 0, 0, 0, 0, 0,              // Color Bits Ignored
      0,                             // No Alpha Buffer
      0,                             // Shift Bit Ignored
      0,                             // No Accumulation Buffer
      0, 0, 0, 0,                    // Accumulation Bits Ignored
      0,                             // depth bits
      0,                             // stencil bits
      0,                             // No Auxiliary Buffer
      PFD_MAIN_PLANE,                // Main Drawing Layer
      0,                             // Reserved
      0, 0, 0                        // Layer Masks Ignored
    };

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = DefWindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandle(NULL);
    wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TmpClass";

    RegisterClassEx(&wcex);

    HWND tmpWND = CreateWindowEx(WS_EX_APPWINDOW, L"TmpClass", L"", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);

    HDC tmpDC = GetDC(tmpWND);
    unsigned int PixelFormat = ChoosePixelFormat(tmpDC, &pfd);
    SetPixelFormat(tmpDC, PixelFormat, &pfd);

    HGLRC tmpRC = wglCreateContext(tmpDC);
    wglMakeCurrent(tmpDC, tmpRC);

    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tmpRC);

    ReleaseDC(tmpWND, tmpDC);
    DestroyWindow(tmpWND);
    UnregisterClass(L"TmpClass", GetModuleHandle(NULL));
  }

  LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM  wParam, LPARAM lParam)
  {
    CrossDelegate *instance;

    if (uMsg == WM_NCCREATE)
    {
      instance = reinterpret_cast<CrossDelegate*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
      SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)instance);
    }
    else
    {
      instance = reinterpret_cast<CrossDelegate*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
    }

    switch (uMsg)
    {
      case WM_CLOSE:
        instance->closeWindow();
        break;

      case WM_PAINT:
        instance->performDraw();
        break;

      case WM_CHAR:
      case WM_SYSCHAR:
      case WM_UNICHAR:
        if (wParam >= 32)
        {
          instance->keyEvents.emplace_back(KeyEvent::KIND_PRESSED, KeyEvent::MODIFIER_NONE, 0, wParam);
        }
        break;

      case WM_SYSKEYDOWN:
      case WM_KEYDOWN:
        instance->handleKeyEvent(KeyEvent::KIND_DOWN, wParam);
        break;

      case WM_SYSKEYUP:
      case WM_KEYUP:
        instance->handleKeyEvent(KeyEvent::KIND_UP, wParam);
        break;

      case WM_LBUTTONDOWN:
        instance->mouseButton = 0;
        instance->mouseEvents.emplace_back(instance->mouseX, instance->mouseY, instance->mouseButton, MouseEvent::KIND_PRESSED);
        break;

      case WM_RBUTTONDOWN:
        instance->mouseButton = 1;
        instance->mouseEvents.emplace_back(instance->mouseX, instance->mouseY, instance->mouseButton, MouseEvent::KIND_PRESSED);
        break;

      case WM_MBUTTONDOWN:
        instance->mouseButton = 2;
        instance->mouseEvents.emplace_back(instance->mouseX, instance->mouseY, instance->mouseButton, MouseEvent::KIND_PRESSED);
        break;

      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP:
        instance->mouseEvents.emplace_back(instance->mouseX, instance->mouseY, instance->mouseButton, MouseEvent::KIND_RELEASED);
        instance->mouseButton = -1;
        break;

      case WM_MOUSEMOVE:
        instance->mouseX = GET_X_LPARAM(lParam);
        instance->mouseY = GET_Y_LPARAM(lParam);

        if (instance->mouseButton != -1)
        {
          instance->mouseEvents.emplace_back(instance->mouseX, instance->mouseY, instance->mouseButton, MouseEvent::KIND_DRAGGED);
        }
        else
        {
          instance->mouseEvents.emplace_back(instance->mouseX, instance->mouseY, -1, MouseEvent::KIND_MOVED);
        }
        break;

      case WM_MOUSEWHEEL:
        instance->wheelEvents.emplace_back(-(SHORT)HIWORD(wParam) / (float)WHEEL_DELTA);
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }


  void CrossDelegate::handleKeyEvent(KeyEvent::Kind kind, int keyCode)
  {
    if ((kind == KeyEvent::KIND_DOWN) && (keyCode == VK_ESCAPE))
    {
      closeWindow();
    }
    else
    {
      int convertedKeyCode = keyCode;
      auto found = KEYMAP.find(keyCode);

      if (found != KEYMAP.end())
      {
        convertedKeyCode = found->second;
      }

      keyEvents.emplace_back(kind, getKeyModifiers(), convertedKeyCode);
    }
  }

  int CrossDelegate::getKeyModifiers()
  {
    int modifiers = 0;

    if (GetKeyState(VK_SHIFT) & (1 << 31)) modifiers |= chr::KeyEvent::MODIFIER_SHIFT;
    if (GetKeyState(VK_CONTROL) & (1 << 31)) modifiers |= chr::KeyEvent::MODIFIER_CTRL;
    if (GetKeyState(VK_MENU) & (1 << 31)) modifiers |= chr::KeyEvent::MODIFIER_ALT;
    if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & (1 << 31)) modifiers |= chr::KeyEvent::MODIFIER_META;

    return modifiers;
  }
}
