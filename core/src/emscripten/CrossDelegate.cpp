
#include "emscripten/CrossDelegate.h"
#include "emscripten/Keyboard.h"

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
  
  bool CrossDelegate::performInit()
  {
    if (!initialized_)
    {
      emscripten_set_resize_callback(0, 0, 1, resizeCallback);

      emscripten_set_mousedown_callback(0, 0, 1, mouseCallback);
      emscripten_set_mouseup_callback(0, 0, 1, mouseCallback);
      emscripten_set_mousemove_callback(0, 0, 1, mouseCallback);

      emscripten_set_keypress_callback(0, 0, 1, keyCallback);
      emscripten_set_keyup_callback(0, 0, 1, keyCallback);
      emscripten_set_keydown_callback(0, 0, 1, keyCallback);

      emscripten_set_wheel_callback(0, 0, 1, wheelCallback);

      // ---

      int canvasWidth = EM_ASM_INT_V("return document.getElementById('canvas').clientWidth");
      int canvasHeight = EM_ASM_INT_V("return document.getElementById('canvas').clientHeight");

      setupInfo.windowInfo = WindowInfo(canvasWidth, canvasHeight, initInfo.windowInfo.aaSamples);

      // ---

      EmscriptenWebGLContextAttributes attr;
      emscripten_webgl_init_context_attributes(&attr);

      attr.alpha = attr.stencil = attr.preserveDrawingBuffer = attr.preferLowPowerToHighPerformance = attr.failIfMajorPerformanceCaveat = 0;
      attr.enableExtensionsByDefault = 1;
      attr.antialias = (initInfo.windowInfo.aaSamples > 0) ? 1 : 0;
      attr.depth = (initInfo.windowInfo.depthBits > 0) ? 1 : 0;
      attr.premultipliedAlpha = 0;
      attr.majorVersion = 1;
      attr.minorVersion = 0;

      EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context(0, &attr);
      emscripten_webgl_make_context_current(ctx);

      // ---

      emscripten_webgl_enable_extension(emscripten_webgl_get_current_context(), "EXT_texture_filter_anisotropic");
      emscripten_webgl_enable_extension(emscripten_webgl_get_current_context(), "WEBKIT_EXT_texture_filter_anisotropic");

      // ---

      intern::instance = this;
      initialized_ = _init();
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

    emscripten_set_canvas_size(size.x, size.y);
    glViewport(0, 0, size.x, size.y);

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
  }

  void CrossDelegate::run(int aaSamples, int depthBits)
  {
    initInfo.windowInfo = WindowInfo(0, 0, aaSamples, depthBits);

    performInit();
    performSetup();
    performResize(setupInfo.windowInfo.size);

    sketch->performStart(CrossSketch::START_REASON_VIEW_SHOWN);

    emscripten_set_main_loop(mainLoopCallback, 0, 1);

    sketch->performStop(CrossSketch::STOP_REASON_VIEW_HIDDEN);

    performShutdown();
    performUninit();
  }

  void CrossDelegate::processMouseEvents()
  {
    for (auto &event : mouseEvents)
    {
      switch (event.kind)
      {
        case MouseEvent::KIND_PRESSED:
          sketch->addTouch(event.button, event.x, event.y);
          break;

        case MouseEvent::KIND_DRAGGED:
          sketch->updateTouch(event.button, event.x, event.y);
          break;

        case MouseEvent::KIND_RELEASED:
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

  void CrossDelegate::mainLoopCallback()
  {
    intern::instance->processMouseEvents();
    intern::instance->processKeyEvents();
    intern::instance->processWheelEvents();

    intern::instance->performUpdate();
    intern::instance->performDraw();

    intern::instance->clearMouseEvents();
    intern::instance->clearKeyEvents();
    intern::instance->clearWheelEvents();
  }

  EM_BOOL CrossDelegate::resizeCallback(int eventType, const EmscriptenUiEvent *e, void *userData)
  {
    int canvasWidth = EM_ASM_INT_V("return document.getElementById('canvas').clientWidth");
    int canvasHeight = EM_ASM_INT_V("return document.getElementById('canvas').clientHeight");

    if ((canvasWidth != intern::instance->setupInfo.windowInfo.width) || (canvasHeight != intern::instance->setupInfo.windowInfo.height))
    {
      intern::instance->performResize(glm::vec2(canvasWidth, canvasHeight));
    }

    return 0;
  }

  EM_BOOL CrossDelegate::mouseCallback(int eventType, const EmscriptenMouseEvent *e, void *userData)
  {
    switch (eventType)
    {
      case EMSCRIPTEN_EVENT_MOUSEDOWN:
      {
        intern::instance->mouseEvents.emplace_back(intern::instance->mouseX, intern::instance->mouseY, e->button, MouseEvent::KIND_PRESSED);

        intern::instance->mouseButton = e->button;
        intern::instance->mousePressed = true;
      }
      break;

      case EMSCRIPTEN_EVENT_MOUSEUP:
      {
        intern::instance->mouseEvents.emplace_back(intern::instance->mouseX, intern::instance->mouseY, e->button, MouseEvent::KIND_RELEASED);

        intern::instance->mouseButton = e->button;
        intern::instance->mousePressed = false;
      }
      break;

      case EMSCRIPTEN_EVENT_MOUSEMOVE:
      {
        intern::instance->mouseEvents.emplace_back(e->clientX, e->clientY, intern::instance->mouseButton, intern::instance->mousePressed ? MouseEvent::KIND_DRAGGED : MouseEvent::KIND_MOVED);

        intern::instance->mouseX = e->clientX;
        intern::instance->mouseY = e->clientY;
      }
      break;
    }

    return 0;
  }

  EM_BOOL CrossDelegate::keyCallback(int eventType, const EmscriptenKeyboardEvent *e, void *userData)
  {
    KeyEvent::Kind kind = KeyEvent::KIND_UNDEFINED;
    int modifiers = KeyEvent::MODIFIER_NONE;
    uint32_t keyCode = 0;
    uint32_t codePoint = 0;

    if (e->shiftKey) modifiers |= KeyEvent::MODIFIER_SHIFT;
    if (e->ctrlKey) modifiers |= KeyEvent::MODIFIER_CTRL;
    if (e->altKey) modifiers |= KeyEvent::MODIFIER_ALT;
    if (e->metaKey) modifiers |= KeyEvent::MODIFIER_META;

    switch (eventType)
    {
      case EMSCRIPTEN_EVENT_KEYPRESS:
        kind = KeyEvent::KIND_PRESSED;
        codePoint = e->keyCode;
        break;

      case EMSCRIPTEN_EVENT_KEYDOWN:
        kind = KeyEvent::KIND_DOWN;
        keyCode = e->keyCode;
        break;

      case EMSCRIPTEN_EVENT_KEYUP:
        kind = KeyEvent::KIND_UP;
        keyCode = e->keyCode;
        break;
     }

    intern::instance->keyEvents.emplace_back(kind, modifiers, intern::instance->convertKeyCode(keyCode), codePoint);

    return 0;
  }

  EM_BOOL CrossDelegate::wheelCallback(int eventType, const EmscriptenWheelEvent *wheelEvent, void *userData)
  {
    intern::instance->wheelEvents.emplace_back(wheelEvent->deltaY);

    return 0;
  }
}
