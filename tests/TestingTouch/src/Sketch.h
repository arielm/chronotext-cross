#pragma once

#include "cross/Context.h"
#include "gl/ShaderProgram.h"

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
  chr::gl::ShaderProgram shaderProgram;
  GLint positionLocation;
  GLint coordLocation;
  GLint colorLocation;
  GLint samplerLocation;
  GLint matrixLocation;

  GLuint vboIds[3];
  GLuint textureIds[1];

  float scale;
  glm::mat4 projectionMatrix;

  std::vector<glm::vec2> dotPositions;

  void initBuffers();
  void initTextures();
  void initShaders();

  void drawDot(const glm::vec2 &position, float radius);
};
