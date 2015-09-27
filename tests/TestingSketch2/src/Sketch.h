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

protected:
  GLuint vboIds[3];
  GLuint textureIds[1];

  GLint maxAnisotropy;
};
