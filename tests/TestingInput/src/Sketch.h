#pragma once

#include "chr/cross/Context.h"

class Sketch : public chr::CrossSketch
{
public:
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

protected:
  void keyPressed(uint32_t codepoint) final;
  void keyDown(int keyCode, int modifiers) final;
  void keyUp(int keyCode, int modifiers) final;

  void mouseMoved(float x, float y);
  void mouseDragged(int button, float x, float y);
  void mousePressed(int button, float x, float y);
  void mouseReleased(int button, float x, float y);

  void wheelUpdated(float offset);
};
