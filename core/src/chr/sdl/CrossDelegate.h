#pragma once

#include "chr/cross/CrossDelegateBase.h"

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
  };
}
