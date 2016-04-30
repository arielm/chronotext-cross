#pragma once

#include "cross/Context.h"
#include "gl/Batch.h"
#include "gl/shaders/TextureAlphaShader.h"

#include <map>

class Sketch : public chr::CrossSketch
{
public:
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

  void addTouch(int index, float x, float y) final;
  void updateTouch(int index, float x, float y) final;

protected:
  chr::gl::Texture texture;
  chr::gl::State textureState;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV> textureBatch;
  chr::gl::shaders::TextureAlphaShader textureAlphaShader;

  float scale;

  std::map<int, glm::vec2> touchPositions;

  void drawDot(const glm::vec2 &position, float radius);
  void initTextures();
};
