#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/ShaderProgram.h"
#include "chr/gl/Camera.h"

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

  chr::gl::ShaderProgram shader;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV> batch;
  chr::gl::InstanceBuffer instanceBuffer;
  chr::gl::Texture texture;

  static void threadHelix(chr::gl::InstanceBuffer &instanceBuffer, float r1, float r2, float turns, float h, float D, float spacing);
};
