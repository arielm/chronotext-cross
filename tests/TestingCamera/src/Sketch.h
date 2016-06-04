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
  void updateTouch(int index, float x, float y) final;
  void removeTouch(int index, float x, float y) final;

protected:
  chr::gl::State state;
  chr::gl::Camera camera;

  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV.RGBA> lightenBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ> contourBatch;
  chr::gl::VertexBatch<chr::gl::XYZ> normalBatch;

  chr::gl::shaders::LambertShader lambertShader;
  chr::gl::shaders::ColorShader colorShader;

  bool mousePressed = false;
  glm::vec2 mousePosition;

  void processRay(const glm::vec2 &position, const chr::gl::Matrix &matrix, int frontFace);
};
