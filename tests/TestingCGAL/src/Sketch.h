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

protected:
  chr::gl::State state;

  chr::gl::IndexedVertexBatch<chr::gl::XYZ> batch1;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ> batch2;

  chr::gl::shaders::ColorShader colorShader;
};