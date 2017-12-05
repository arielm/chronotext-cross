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

  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV> colorBatch;
  chr::gl::shaders::ColorShader colorShader;

  GLuint fboId;
  chr::gl::Texture fboColorTexture;
  chr::gl::Texture fboDepthTexture;
};
