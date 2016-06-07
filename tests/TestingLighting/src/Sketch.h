#pragma once

#include "cross/Context.h"
#include "gl/Camera.h"
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

  void addTouch(int index, float x, float y) final;
  void removeTouch(int index, float x, float y) final;

protected:
  chr::gl::State state;
  chr::gl::Camera camera;

  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.RGBA> fillBatch;
  chr::gl::VertexBatch<chr::gl::XYZ> normalBatch;

  chr::gl::ShaderProgram phongShader;
  chr::gl::shaders::LambertShader lambertShader;
  chr::gl::shaders::ColorShader colorShader;

  bool pressed = false;
};
