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
  chr::gl::shaders::ColorShader colorShader;

  chr::gl::IndexedVertexBatch<chr::gl::XYZ> girlBatch;
  chr::gl::VertexBatch<chr::gl::XYZ> lysBatch;

  void createLys();
  void createGirl();

  static void drawPolyline(chr::gl::VertexBatch<chr::gl::XYZ> &batch, const std::vector<glm::vec2> &polyline);
};
