#include "desktop/CrossDelegate.h"
#include "desktop/Keyboard.h"

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
      if (glfwInit())
      {
        glfwWindowHint(GLFW_SAMPLES, initInfo.windowInfo.aaSamples);
        glfwWindowHint(GLFW_DEPTH_BITS, initInfo.windowInfo.depthBits);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        // ---

        int targetWidth;
        int targetHeight;

        if (initInfo.windowInfo.size.x * initInfo.windowInfo.size.y == 0)
        {
          GLFWmonitor* monitor = glfwGetPrimaryMonitor();

          if (monitor)
          {
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);

            targetWidth = mode->width;
            targetHeight = mode->height;

            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            initInfo.window = glfwCreateWindow(targetWidth, targetHeight, "", monitor, NULL);
          }
        }
        else
        {
          targetWidth = initInfo.windowInfo.width;
          targetHeight = initInfo.windowInfo.height;

          initInfo.window = glfwCreateWindow(targetWidth, targetHeight, "", NULL, NULL);
        }

        if (initInfo.window)
        {
          setupInfo.windowInfo = WindowInfo(targetWidth, targetHeight, initInfo.windowInfo.aaSamples, initInfo.windowInfo.depthBits);

          glfwSetCursorPosCallback(initInfo.window, cursorPosCallback);
          glfwSetMouseButtonCallback(initInfo.window, mouseButtonCallback);
          glfwSetKeyCallback(initInfo.window, keyCallback);
          glfwSetCharCallback(initInfo.window, characterCallback);

          glfwMakeContextCurrent(initInfo.window);
          gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

          // ---

          intern::instance = this;
          initialized_ = _init();
        }
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
  }

  void CrossDelegate::run(int width, int height, int aaSamples, int depthBits)
  {
    initInfo.windowInfo = WindowInfo(width, height, aaSamples, depthBits);

    performInit(); // TODO: HANDLE FAILURES
    performSetup();
    performResize(setupInfo.windowInfo.size);

    sketch->performStart(CrossSketch::START_REASON_VIEW_SHOWN);

    while (!glfwWindowShouldClose(initInfo.window))
    {
      intern::instance->processMouseEvents();
      intern::instance->processKeyEvents();

      performUpdate();
      performDraw();

      glfwSwapBuffers(initInfo.window);

      intern::instance->clearMouseEvents();
      intern::instance->clearKeyEvents();
      glfwPollEvents();
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

  void CrossDelegate::cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
  {
    intern::instance->mouseEvents.emplace_back(xpos, ypos, intern::instance->mouseButton, intern::instance->mousePressed ? MouseEvent::KIND_DRAGGED : MouseEvent::KIND_MOVED);

    intern::instance->mouseX = xpos;
    intern::instance->mouseY = ypos;
  }

  void CrossDelegate::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
  {
    intern::instance->mouseEvents.emplace_back(intern::instance->mouseX, intern::instance->mouseY, button, (action == GLFW_PRESS) ? MouseEvent::KIND_PRESSED : MouseEvent::KIND_RELEASED);

    intern::instance->mouseButton = button;
    intern::instance->mousePressed = (action == GLFW_PRESS);
  }

  void CrossDelegate::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
  {
    if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
    {
      glfwSetWindowShouldClose(window, true);
    }
    else
    {
      KeyEvent::Kind kind = KeyEvent::KIND_UNDEFINED;
      int modifiers = KeyEvent::MODIFIER_NONE;

      switch (action)
      {
        case GLFW_PRESS:
        case GLFW_REPEAT:
          kind = KeyEvent::KIND_DOWN;
          break;

        case GLFW_RELEASE:
          kind = KeyEvent::KIND_UP;
          break;
      }

      if (mods & GLFW_MOD_SHIFT) modifiers |= KeyEvent::MODIFIER_SHIFT;
      if (mods & GLFW_MOD_CONTROL) modifiers |= KeyEvent::MODIFIER_CTRL;
      if (mods & GLFW_MOD_ALT) modifiers |= KeyEvent::MODIFIER_ALT;
      if (mods & GLFW_MOD_SUPER) modifiers |= KeyEvent::MODIFIER_META;

      intern::instance->keyEvents.emplace_back(kind, modifiers, intern::instance->convertKeyCode(key));
    }
  }

  void CrossDelegate::characterCallback(GLFWwindow *window, unsigned int codePoint)
  {
    intern::instance->keyEvents.emplace_back(KeyEvent::KIND_PRESSED, KeyEvent::MODIFIER_NONE, 0, codePoint);
  }
}
