#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/shaders/ColorShader.h"

class Sketch : public chr::CrossSketch
{
public:
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

  void keyPressed(uint32_t codepoint) final;
  void keyDown(int keyCode, int modifiers) final;
  void keyUp(int keyCode, int modifiers) final;

  void mouseMoved(float x, float y);
  void mouseDragged(int button, float x, float y);
  void mousePressed(int button, float x, float y);
  void mouseReleased(int button, float x, float y);

  void wheelUpdated(float offset);

protected:
  chr::gl::State state;
  chr::gl::shaders::ColorShader colorShader;
};
