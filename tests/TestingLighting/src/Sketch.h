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

  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.RGBA> faceBatch;
  chr::gl::VertexBatch<chr::gl::XYZ> normalBatch;

  chr::gl::ShaderProgram phongShader;
  chr::gl::ShaderProgram lambertShader;
  chr::gl::shaders::ColorShader colorShader;

  bool pressed = false;
};
