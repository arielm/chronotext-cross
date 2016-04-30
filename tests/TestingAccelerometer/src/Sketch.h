#pragma once

#include "cross/Context.h"
#include "gl/Batch.h"
#include "gl/shaders/TextureAlphaShader.h"

struct Particle
{
    glm::vec2 position;
    glm::vec2 previousPosition;
    glm::vec2 acceleration;

    float radius;
    float mass;

    Particle() = default;

    Particle(const glm::vec2 &position, float radius, float mass = 1)
    :
    position(position),
    previousPosition(position),
    radius(radius),
    mass(mass)
    {}
};

class Sketch : public chr::CrossSketch
{
public:
  virtual ~Sketch() {}

  void setup() final;

  void start(StartReason reason) final;
  void stop(StopReason reason) final;

  void update() final;
  void draw() final;

  void accelerated(chr::AccelEvent event) final;

protected:
  chr::gl::Texture texture;
  chr::gl::State textureState;
  chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV> textureBatch;
  chr::gl::shaders::TextureAlphaShader textureAlphaShader;

  float scale;

  glm::vec2 acceleration;
  Particle particle;

  void drawDot(const glm::vec2 &position, float radius);

  void accumulateForces();
  void verlet();
  void satisfyConstraints();

  void initTextures();
};
