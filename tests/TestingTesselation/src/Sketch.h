#pragma once

#include "cross/Context.h"
#include "gl/Batch.h"
#include "gl/shaders/ColorShader.h"
#include "gl/shaders/LambertShader.h"

class Sketch : public chr::CrossSketch
{
public:
  Sketch();
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

protected:
  chr::gl::State state;

  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.RGBA> faceBatch;
  chr::gl::VertexBatch<chr::gl::XYZ> normalBatch;

  chr::gl::shaders::LambertShader lambertShader;
  chr::gl::shaders::ColorShader colorShader;
};
