#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/ShaderProgram.h"
#include "chr/gl/Camera.h"

#include "SeaSurfaceSpiral2.h"

class Sketch : public chr::CrossSketch
{
public:
  Sketch();

  void setup() final;
  void resize() final;
  void update() final;
  void draw() final;

protected:
  chr::gl::Camera camera;

  chr::gl::ShaderProgram shader;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV> batch;
  chr::gl::InstanceBuffer instanceBuffer;
  chr::gl::Texture texture;

  SeaSurface surface;
  SeaSurfaceSpiral2 spiral;

  static void threadSpiral(chr::gl::InstanceBuffer &instanceBuffer, const chr::path::FollowablePath3D &path, float spacing);
};
