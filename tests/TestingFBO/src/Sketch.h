#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/shaders/TextureShader.h"
#include "chr/gl/shaders/TextureAlphaShader.h"
#include "chr/gl/shaders/LambertShader.h"
#include "chr/gl/FBO.h"

class Sketch : public chr::CrossSketch
{
public:
  Sketch();
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

protected:
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV.RGBA> textureBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV> lightenBatch;
  chr::gl::shaders::TextureShader textureShader;
  chr::gl::shaders::TextureAlphaShader textureAlphaShader;
  chr::gl::shaders::LambertShader lambertShader;

  chr::gl::Texture texture;
  chr::gl::FBO fbo;

  void drawScene1();
  void drawScene2(const glm::ivec2 &size);
};
