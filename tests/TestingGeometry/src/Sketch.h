#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Camera.h"
#include "chr/gl/Batch.h"
#include "chr/gl/ShaderProgram.h"

class Sketch : public chr::CrossSketch
{
public:
  Sketch();
  virtual ~Sketch() {}

  void setup() final;
  void resize() final;
  void draw() final;

protected:
  chr::gl::Camera camera;

  chr::gl::Texture texture;
  chr::gl::ShaderProgram shader;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV.RGBA> geometryBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV> torusBatch;
};
