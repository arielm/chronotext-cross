#pragma once

#include "cross/Context.h"
#include "gl/Batch.h"
#include "gl/shaders/ColorShader.h"

class Sketch : public chr::CrossSketch
{
public:
  Sketch();
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

  void keyPressed(uint32_t codepoint) final;
  void keyDown(int keyCode, int modifiers) final;
  void keyUp(int keyCode, int modifiers) final;

  void wheelUpdated(float offset);

protected:
  chr::gl::State state;
  chr::gl::shaders::ColorShader colorShader;
};
