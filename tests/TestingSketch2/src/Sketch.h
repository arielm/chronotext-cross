#pragma once

#include "cross/Context.h"

class Sketch : public chr::CrossSketch
{
public:
  virtual ~Sketch() {}

  void setup() final;
  void shutdown() final;
  void draw() final;

  void initBuffers();
  void initTextures();
  void initShaders();

protected:
  GLuint shaderProgram;
  GLint positionLocation;
  GLint coordLocation;
  GLint colorLocation;
  GLint samplerLocation;
  GLint matrixLocation;

  GLuint vboIds[3];
  GLuint textureIds[1];

  GLint maxAnisotropy;
};
