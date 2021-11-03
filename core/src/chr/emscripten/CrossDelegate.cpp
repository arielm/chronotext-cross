#include "chr/emscripten/CrossDelegate.h"
#include "chr/emscripten/Keyboard.h"

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

      emscripten_set_touchstart_callback(0, 0, 1, touchCallback);
      emscripten_set_touchend_callback(0, 0, 1, touchCallback);
      emscripten_set_touchmove_callback(0, 0, 1, touchCallback);
      emscripten_set_touchcancel_callback(0, 0, 1, touchCallback);

      emscripten_set_keypress_callback(0, 0, 1, keyCallback);
      emscripten_set_keyup_callback(0, 0, 1, keyCallback);
      emscripten_set_keydown_callback(0, 0, 1, keyCallback);

      emscripten_set_wheel_callback(0, 0, 1, wheelCallback);

      emscripten_set_focus_callback(0, 0, 1, focusCallback);
      emscripten_set_blur_callback(0, 0, 1, blurCallback);

      // ---

      measureCanvas();
      setupInfo.windowInfo = WindowInfo(canvasSize.x, canvasSize.y, initInfo.windowInfo.aaSamples);

      // ---

      if (EM_ASM_INT_V("return /android/i.test(navigator.userAgent)"))
      {
        browserPlatform = BROWSER_PLATFORM_ANDROID;
      }
      else if (EM_ASM_INT_V("return /iPad|iPhone|iPod/i.test(navigator.userAgent)"))
      {
        browserPlatform = BROWSER_PLATFORM_IOS;
      }

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

  void CrossDelegate::enableAccelerometer(float updateFrequency, float filterFactor)
  {
    accelFilter = AccelEvent::Filter(filterFactor);
    emscripten_set_devicemotion_callback(0, 1, deviceMotionCallback);
  }

  void CrossDelegate::disableAccelerometer()
  {
    emscripten_set_devicemotion_callback(0, 1, NULL);
  }

  void CrossDelegate::measureCanvas()
  {
    int width = EM_ASM_INT_V("return document.getElementById('canvas').clientWidth");
    int height = EM_ASM_INT_V("return document.getElementById('canvas').clientHeight");
    canvasSize = glm::ivec2(width, height);
  }

  void CrossDelegate::processMouseEvents()
  {
    for (auto &event : mouseEvents)
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

  void CrossDelegate::processTouchEvents()
  {
    for (auto &event : touchEvents)
    {
      switch (event.kind)
      {
        case TouchEvent::KIND_ADD:
          sketch->addTouch(event.id, event.x, event.y);
          break;

        case TouchEvent::KIND_UPDATE:
          sketch->updateTouch(event.id, event.x, event.y);
          break;

        case TouchEvent::KIND_REMOVE:
          sketch->removeTouch(event.id, event.x, event.y);
          break;

        default:
          break;
      }
    }
  }

  void CrossDelegate::clearTouchEvents()
  {
    touchEvents.clear();
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

  void CrossDelegate::processAccelerationEvents()
  {
    for (const auto &event : accelerationEvents)
    {
      sketch->accelerated(event);
    }
  }

  void CrossDelegate::clearAccelerationEvents()
  {
    accelerationEvents.clear();
  }

  void CrossDelegate::mainLoopCallback()
  {
    intern::instance->processMouseEvents();
    intern::instance->processTouchEvents();
    intern::instance->processKeyEvents();
    intern::instance->processWheelEvents();
    intern::instance->processAccelerationEvents();

    intern::instance->performUpdate();
    intern::instance->performDraw();

    intern::instance->clearMouseEvents();
    intern::instance->clearTouchEvents();
    intern::instance->clearKeyEvents();
    intern::instance->clearWheelEvents();
    intern::instance->clearAccelerationEvents();
  }

  EM_BOOL CrossDelegate::resizeCallback(int eventType, const EmscriptenUiEvent *e, void *userData)
  {
    intern::instance->measureCanvas();
    int canvasWidth = intern::instance->canvasSize.x;
    int canvasHeight = intern::instance->canvasSize.y;

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
        intern::instance->mouseX = e->canvasX;
        intern::instance->mouseY = e->canvasY;

        intern::instance->mouseEvents.emplace_back(intern::instance->mouseX, intern::instance->mouseY, intern::instance->mouseButton, intern::instance->mousePressed ? MouseEvent::KIND_DRAGGED : MouseEvent::KIND_MOVED);
      }
      break;
    }

    return 0;
  }

  EM_BOOL CrossDelegate::touchCallback(int eventType, const EmscriptenTouchEvent *e, void *userData)
  {
    for (int i = 0; i < e->numTouches; ++i)
    {
      const EmscriptenTouchPoint *t = &e->touches[i];

      if (t->isChanged)
      {
        int id = t->identifier;
        float x = t->canvasX;
        float y = t->canvasY;

        switch (eventType)
        {
          case EMSCRIPTEN_EVENT_TOUCHSTART:
            intern::instance->touchEvents.emplace_back(x, y, id, TouchEvent::KIND_ADD);
            break;

          case EMSCRIPTEN_EVENT_TOUCHMOVE:
            intern::instance->touchEvents.emplace_back(x, y, id, TouchEvent::KIND_UPDATE);
            break;

          case EMSCRIPTEN_EVENT_TOUCHEND:
          case EMSCRIPTEN_EVENT_TOUCHCANCEL:
            intern::instance->touchEvents.emplace_back(x, y, id, TouchEvent::KIND_REMOVE);
            break;
        }
      }
    }

    return 1;
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
        if (e->charCode >= 32)
        {
          kind = KeyEvent::KIND_PRESSED;
          codePoint = e->charCode;
          break;
        }
        else
        {
          return 0;
        }

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

  EM_BOOL CrossDelegate::wheelCallback(int eventType, const EmscriptenWheelEvent *e, void *userData)
  {
    intern::instance->wheelEvents.emplace_back(e->deltaY);

    return 0;
  }

  EM_BOOL CrossDelegate::focusCallback(int eventType, const EmscriptenFocusEvent *e, void *userData)
  {
    intern::instance->sketch->performStart(CrossSketch::START_REASON_APP_RESUMED);
    
    return 0;
  }

  EM_BOOL CrossDelegate::blurCallback(int eventType, const EmscriptenFocusEvent *e, void *userData)
  {
    intern::instance->sketch->performStop(CrossSketch::STOP_REASON_APP_PAUSED);

    return 0;
  }

  EM_BOOL CrossDelegate::deviceMotionCallback(int eventType, const EmscriptenDeviceMotionEvent *e, void *userData)
  {
    glm::vec3 acceleration(e->accelerationIncludingGravityX, e->accelerationIncludingGravityY, e->accelerationIncludingGravityZ);

    if (intern::instance->browserPlatform == BROWSER_PLATFORM_ANDROID)
    {
      acceleration *= -1;
    }

    intern::instance->accelerationEvents.emplace_back(intern::instance->accelFilter.process(acceleration));

    return 0;
  }
}
