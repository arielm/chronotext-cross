#pragma once

#include "cross/Context.h"
#include "gl/Batch.h"
#include "gl/TextureShader.h"
#include "gl/TextureAlphaShader.h"

class Sketch : public chr::CrossSketch
{
public:
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

  void initTextures();

protected:
  chr::gl::Texture textures[3];
  chr::gl::IndexedVertexBatch<chr::gl::UV|chr::gl::RGBA> textureBatches[3];

  chr::gl::TextureShader textureShader;
  chr::gl::TextureAlphaShader textureAlphaShader;
};
