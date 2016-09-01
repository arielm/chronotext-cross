#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/shaders/ColorShader.h"
#include "chr/gl/shaders/LambertShader.h"
#include "chr/gl/shaders/TextureAlphaShader.h"

class Sketch : public chr::CrossSketch
{
public:
  Sketch();
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

protected:
  chr::gl::State state2D;
  chr::gl::State state3D;

  chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV> dotBatch;
  chr::gl::VertexBatch<chr::gl::XYZ> lineBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ> contourBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ> flatBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV.RGBA> lightenBatch;

  chr::gl::shaders::ColorShader colorShader;
  chr::gl::shaders::LambertShader lambertShader;
  chr::gl::shaders::TextureAlphaShader textureAlphaShader;

  chr::gl::Texture dotTexture;
  chr::gl::Texture lysTexture;

  void drawPolyline(const std::vector<glm::vec2> &polyline);
  void drawDot(const glm::vec2 &position, float radius);

  void initTextures();
};
