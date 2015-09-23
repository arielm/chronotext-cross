#pragma once

#include "cross/CrossSketch.h"

class Sketch : public chr::CrossSketch
{
public:
  virtual ~Sketch() {}

  void setup() final;
  void shutdown() final;
  void draw() final;

protected:
  GLuint shaderProgram;
  GLint positionLocation;
  GLint coordLocation;
  GLint colorLocation;
  GLint samplerLocation;
  GLint matrixLocation;

  GLuint vboIds[3];
  GLuint textureIds[1];

  float scale;
  glm::mat4 projectionMatrix;

  void initBuffers();
  void initTextures();
  void initShaders();

  void drawDot(const glm::vec2 &position, float radius);
};
