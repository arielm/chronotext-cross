#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/shaders/TextureShader.h"
#include "chr/gl/shaders/TextureAlphaShader.h"
#include "chr/gl/shaders/LambertShader.h"

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

  GLuint fboId;
  chr::gl::Texture fboColorTexture;
  chr::gl::Texture fboDepthTexture;

  void setupFramebuffer();
  void drawScene1();
  void drawScene2();
};
