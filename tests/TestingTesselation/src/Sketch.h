#pragma once

#include "cross/Context.h"
#include "gl/Batch.h"
#include "gl/shaders/ColorShader.h"
#include "gl/shaders/LambertShader.h"
#include "gl/shaders/TextureAlphaShader.h"

class Sketch : public chr::CrossSketch
{
public:
  Sketch();
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

protected:
  chr::gl::State state;

  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV.RGBA> lightenBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.RGBA> flatBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ> contourBatch;
  chr::gl::VertexBatch<chr::gl::XYZ> normalBatch;

  chr::gl::shaders::LambertShader lambertShader;
  chr::gl::shaders::ColorShader colorShader;
  chr::gl::shaders::TextureAlphaShader textureAlphaShader;

  chr::gl::Texture texture;

  void initTextures();
};
