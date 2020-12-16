#include "chr/sdl/CrossDelegate.h"

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
      SDL_Init(SDL_INIT_VIDEO);

      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
      SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
      SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
      SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
      SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
      SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
      SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, initInfo.windowInfo.depthBits);
      SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // XXX

      if (initInfo.windowInfo.aaSamples > 0)
      {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, initInfo.windowInfo.aaSamples);
      }

      int targetWidth;
      int targetHeight;

      if (initInfo.windowInfo.size.x * initInfo.windowInfo.size.y == 0)
      {
        SDL_DisplayMode mode;
        SDL_GetDesktopDisplayMode(0, &mode);

        targetWidth = mode.w;
        targetHeight = mode.h;

        window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
      }
      else
      {
        targetWidth = initInfo.windowInfo.width;
        targetHeight = initInfo.windowInfo.height;

        window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, targetWidth, targetHeight, SDL_WINDOW_OPENGL);
      }

      setupInfo.windowInfo = WindowInfo(targetWidth, targetHeight, initInfo.windowInfo.aaSamples, initInfo.windowInfo.depthBits);

      glContext = SDL_GL_CreateContext(window);
      SDL_GL_SetSwapInterval(1);

      glewInit();

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

  void CrossDelegate::run(int width, int height, int aaSamples, int depthBits)
  {
    initInfo.windowInfo = WindowInfo(width, height, aaSamples, depthBits);

    performInit(); // TODO: HANDLE FAILURES
    performSetup();
    performResize(setupInfo.windowInfo.size);

    sketch->performStart(CrossSketch::START_REASON_VIEW_SHOWN);

    bool close = false;
    do
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                close = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    close = true;
                }
            }
        }

        performUpdate();
        performDraw();

        SDL_GL_SwapWindow(window);
    }
    while (!close);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    sketch->performStop(CrossSketch::STOP_REASON_VIEW_HIDDEN);

    performShutdown();
    performUninit();
  }

  void CrossDelegate::hideCursor()
  {
    SDL_ShowCursor(SDL_DISABLE);
  }

  void CrossDelegate::showCursor()
  {
    SDL_ShowCursor(SDL_ENABLE);
  }
}
