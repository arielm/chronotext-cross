#pragma once

#include "cross/Context.h"
#include "gl/Batch.h"
#include "gl/shaders/TextureShader.h"
#include "gl/shaders/TextureAlphaShader.h"

class Sketch : public chr::CrossSketch
{
public:
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

  void initTextures();

protected:
  chr::gl::Texture textures[3];
  chr::gl::State states[3];
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV> textureBatches[3];

  chr::gl::shaders::TextureShader textureShader;
  chr::gl::shaders::TextureAlphaShader textureAlphaShader;
};
