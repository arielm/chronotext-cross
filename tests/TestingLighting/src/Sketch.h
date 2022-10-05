#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Camera.h"
#include "chr/gl/Batch.h"
#include "chr/gl/ShaderProgram.h"
#include "chr/gl/shaders/ColorShader.h"

class Sketch : public chr::CrossSketch
{
public:
  Sketch();

  void setup() final;
  void resize() final;
  void draw() final;

protected:
  chr::gl::Camera camera;

  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N> modelBatch;
  chr::gl::ShaderProgram shader;

  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N> lightBatch;
  chr::gl::shaders::ColorShader colorShader;
};
