#pragma once

#include "cross/Context.h"
#include "gl/TextureHandle.h"

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
  GLuint vboIds[3];
  chr::gl::TextureHandle texture;

  float scale;
  glm::mat4 projectionMatrix;

  std::vector<glm::vec2> dotPositions;

  void initBuffers();
  void initTextures();

  void drawDot(const glm::vec2 &position, float radius);
};
