#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/shaders/ColorShader.h"
#include "chr/gl/shaders/TextureShader.h"
#include "chr/path/SVGDocument.h"

class Sketch : public chr::CrossSketch
{
public:
  Sketch();
  virtual ~Sketch() {}

  void setup() final;
  void resize() final;
  void draw() final;

protected:
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.RGBA> gradientBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ> starsBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ> moonBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV> ufoBatch;

  chr::gl::shaders::ColorShader colorShader;
  chr::gl::shaders::TextureShader textureShader;

  chr::path::SVGDocument moonDocument;
  chr::gl::Texture ufoTexture;

  void createGradient();
  void createStars(int n);
  void loadMoon();
  void placeMoon();
  void loadUFO();
  void drawUFO();
};
