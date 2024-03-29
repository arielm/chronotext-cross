#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/shaders/LambertShader.h"
#include "chr/gl/shaders/ColorShader.h"

class Sketch : public chr::CrossSketch
{
public:
  void setup() final;
  void draw() final;

protected:
  chr::gl::shaders::LambertShader lambertShader;
  chr::gl::shaders::ColorShader colorShader;

  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N> sunBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N> earthBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N> moonBatch;
  chr::gl::VertexBatch<chr::gl::XYZ> pathBatch;

  void createPath();
};
