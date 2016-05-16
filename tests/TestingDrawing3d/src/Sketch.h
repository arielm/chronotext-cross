#pragma once

#include "cross/Context.h"
#include "gl/Batch.h"
#include "gl/shaders/LambertShader.h"

class Sketch : public chr::CrossSketch
{
public:
  Sketch();
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

  void initTextures();

protected:
  chr::gl::State state;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N> xyzBatch;
  chr::gl::shaders::LambertShader lambertShader;
};
