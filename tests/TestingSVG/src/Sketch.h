#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/shaders/ColorShader.h"

class Sketch : public chr::CrossSketch
{
public:
  Sketch();
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

protected:
  chr::gl::State state;

  chr::gl::IndexedVertexBatch<chr::gl::XYZ.RGBA> fillBatch;
  chr::gl::VertexBatch<chr::gl::XYZ> strokeBatch;

  chr::gl::shaders::ColorShader colorShader;

  void drawPolyline(const std::vector<glm::vec2> &polyline);
};
