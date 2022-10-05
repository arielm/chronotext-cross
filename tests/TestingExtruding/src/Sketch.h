#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/shaders/ColorShader.h"
#include "chr/gl/shaders/TextureLambertShader.h"

class Sketch : public chr::CrossSketch
{
public:
  void setup() final;
  void draw() final;

protected:
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV.RGBA> lightenBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.RGBA> flatBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ> contourBatch;

  chr::gl::shaders::TextureLambertShader textureLambertShader;
  chr::gl::shaders::ColorShader colorShader;

  chr::gl::Texture texture;
};
