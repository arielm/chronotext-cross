#pragma once

#include "cross/Context.h"
#include "gl/TextureBuffer.h"

class Sketch : public chr::CrossSketch
{
public:
  virtual ~Sketch() {}

  void setup() final;
  void shutdown() final;
  void draw() final;

  void addTouch(int index, float x, float y) final;
  void updateTouch(int index, float x, float y) final;

protected:
  chr::gl::TextureHandle texture;
  chr::gl::TextureBuffer textureBuffer;

  float scale;
  glm::mat4 projectionMatrix;

  std::vector<glm::vec2> dotPositions;

  void drawDot(const glm::vec2 &position, float radius);
  void initTextures();
};
