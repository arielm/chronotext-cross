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

  void initTextures();

protected:
  chr::gl::TextureBuffer textureBuffer;
  chr::gl::TextureHandle texture;
};
