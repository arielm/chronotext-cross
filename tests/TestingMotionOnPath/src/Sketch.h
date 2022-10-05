#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/shaders/ColorShader.h"
#include "chr/gl/shaders/TextureAlphaShader.h"
#include "chr/path/FollowablePath2D.h"

class Sketch : public chr::CrossSketch
{
public:
  void setup() final;
  void draw() final;

protected:
  chr::path::FollowablePath2D path1, path2, path3;

  chr::gl::Texture dotTexture;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV> dotBatch;

  chr::gl::VertexBatch<chr::gl::XYZ> lineBatch;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.RGBA> flatBatch;

  chr::gl::shaders::ColorShader colorShader;
  chr::gl::shaders::TextureAlphaShader textureAlphaShader;

  void drawPolyline(const std::vector<glm::vec2> &polyline);
  void drawPolyline(const std::vector<chr::path::FollowablePath2D::Point> &points);
  void drawDot(const glm::vec2 &position, float radius);
};
