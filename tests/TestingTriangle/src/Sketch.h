#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/shaders/ColorShader.h"

class Sketch : public chr::CrossSketch
{
public:
  void setup() final;
  void draw() final;

protected:
  chr::gl::shaders::ColorShader colorShader;
  chr::gl::VertexBatch<chr::gl::XYZ.RGBA> batch;
};
