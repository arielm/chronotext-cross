#pragma once

#include "cross/Context.h"
#include "gl/Batch.h"
#include "gl/shaders/ColorShader.h"
#include "gl/shaders/TextureAlphaShader.h"

class Sketch : public chr::CrossSketch
{
public:
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

  void initTextures();

protected:
  chr::gl::Texture texture;
  chr::gl::State textureState;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV.RGBA> textureBatch;
  chr::gl::shaders::TextureAlphaShader textureAlphaShader;

  chr::gl::State colorState;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.RGBA> colorBatch;
  chr::gl::shaders::TextureAlphaShader colorShader;
};
