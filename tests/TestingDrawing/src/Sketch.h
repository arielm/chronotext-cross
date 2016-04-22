#pragma once

#include "cross/Context.h"
#include "gl/Batch.h"
#include "gl/ColorShader.h"
#include "gl/TextureAlphaShader.h"

class Sketch : public chr::CrossSketch
{
public:
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

  void initTextures();

protected:
  chr::gl::Texture texture;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV.RGBA> textureBatch;
  chr::gl::TextureAlphaShader textureAlphaShader;

  chr::gl::IndexedVertexBatch<chr::gl::XYZ.RGBA> colorBatch;
  chr::gl::TextureAlphaShader colorShader;
};
