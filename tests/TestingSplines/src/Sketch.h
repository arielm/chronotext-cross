#pragma once

#include "cross/Context.h"
#include "gl/Batch.h"
#include "gl/shaders/ColorShader.h"
#include "gl/shaders/TextureAlphaShader.h"
#include "path/FollowablePath2D.h"

class Sketch : public chr::CrossSketch
{
public:
  Sketch();
  virtual ~Sketch() {}

  void setup() final;
  void shutdown() final;
  void draw() final;

protected:
  chr::path::FollowablePath2D path1, path2, path3;

  chr::gl::Texture dotTexture;
  chr::gl::State dotState;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV> dotBatch;

  chr::gl::State lineState;
  chr::gl::VertexBatch<chr::gl::XYZ> lineBatch;

  chr::gl::State flatState;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.RGBA> flatBatch;

  chr::gl::shaders::ColorShader colorShader;
  chr::gl::shaders::TextureAlphaShader textureAlphaShader;

  void drawPolyline(const std::vector<glm::vec2> &polyline);
  void drawPolyline(const std::vector<chr::path::FollowablePath2D::Point> &points);
  void drawDot(const glm::vec2 &position, float radius);

  void initTextures();
};
