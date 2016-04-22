#pragma once

#include "cross/Context.h"
#include "gl/Batch.h"
#include "gl/TextureAlphaShader.h"

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
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV> textureBatch;
  chr::gl::TextureAlphaShader textureAlphaShader;

  float scale;
  glm::mat4 projectionMatrix;

  std::map<int, glm::vec2> touchPositions;

  void drawDot(const glm::vec2 &position, float radius);
  void initTextures();
};
