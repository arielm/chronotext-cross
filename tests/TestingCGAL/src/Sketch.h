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

  void addTouch(int index, float x, float y) final;
  void removeTouch(int index, float x, float y) final;

protected:
  chr::gl::State state;

  chr::gl::IndexedVertexBatch<chr::gl::XYZ> batch1;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ> batch2;
  chr::gl::VertexBatch<chr::gl::XYZ> batch3;

  chr::gl::shaders::ColorShader colorShader;

  bool pressed = false;
};
