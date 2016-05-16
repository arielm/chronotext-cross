#pragma once

#include "cross/Context.h"
#include "gl/Batch.h"
#include "gl/Matrix.h"
#include "gl/shaders/ColorShader.h"
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

  chr::gl::IndexedVertexBatch<chr::gl::XYZ.RGBA> backgroundBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.RGBA> foregroundBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ> strokeBatch;

  chr::gl::shaders::TextureAlphaShader textureAlphaShader;
  chr::gl::shaders::ColorShader colorShader;

  chr::gl::Texture texture;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV.RGBA> textureBatch;
  chr::gl::Matrix textureMatrix;

  void initTextures();
};
