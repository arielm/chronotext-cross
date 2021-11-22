#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/Matrix.h"
#include "chr/gl/shaders/ColorShader.h"
#include "chr/gl/shaders/TextureAlphaShader.h"

class Sketch : public chr::CrossSketch
{
public:
  Sketch();
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

protected:
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
