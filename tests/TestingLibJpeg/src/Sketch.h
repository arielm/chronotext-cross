#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/shaders/TextureShader.h"

class Sketch : public chr::CrossSketch
{
public:
  void setup() final;
  void draw() final;

protected:
  chr::gl::shaders::TextureShader textureShader;

  chr::gl::Texture textures[2];
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV> textureBatches[2];

  void loadTextures();
};
