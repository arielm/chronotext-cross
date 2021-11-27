#include "Sketch.h"

#include "chr/gl/Matrix.h"
#include "chr/gl/draw/Sprite.h"

using namespace std;
using namespace chr;
using namespace gl;

static constexpr float DOT_RADIUS_DP = 22;
static constexpr float DOT_RADIUS_PIXELS = 56; // SPECIFIC TO "dot_112.png"

static constexpr float FRICTION = 0.01f;
static constexpr float DT = 1.0f;

void Sketch::setup()
{
  scale = getDisplayInfo().density / DisplayInfo::REFERENCE_DENSITY;
  particle = Particle(windowInfo.size * 0.5f, scale * DOT_RADIUS_DP);

  texture = Texture(Texture::ImageRequest("dot_112.png")
    .setFlags(image::FLAGS_TRANSLUCENT)
    .setMipmap(true));

  textureBatch
    .setShader(textureAlphaShader)
    .setShaderColor(1, 1, 1, 1)
    .setTexture(texture);

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::start(StartReason reason)
{
  acceleration = glm::vec2(0);
  delegate().enableAccelerometer(15);
}

void Sketch::stop(StopReason reason)
{
  delegate().disableAccelerometer();
}

void Sketch::update()
{
  accumulateForces();
  verlet();
  satisfyConstraints();
}

void Sketch::draw()
{
  glClearColor(0, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  auto projectionMatrix = glm::ortho(0.0f, windowInfo.width, windowInfo.height, 0.0f);

  State()
    .setShaderMatrix(projectionMatrix)
    .apply();

  textureBatch.clear();
  drawDot(particle.position, particle.radius);
  textureBatch.flush();
}

void Sketch::accelerated(AccelEvent event)
{
  acceleration = glm::vec2(+event.getRawData().x, -event.getRawData().y);
}

void Sketch::drawDot(const glm::vec2 &position, float radius)
{
  draw::Sprite()
    .setAnchor(0.5f, 0.5f)
    .append(textureBatch, Matrix()
      .translate(position)
      .scale(radius / DOT_RADIUS_PIXELS));
}

// ---

void Sketch::accumulateForces()
{
  particle.acceleration = particle.mass * acceleration;
}

void Sketch::verlet()
{
  auto tmp = particle.position;
  particle.position = (2 - FRICTION) * particle.position - (1 - FRICTION) * particle.previousPosition + particle.acceleration * DT * DT;
  particle.previousPosition = tmp;
}

void Sketch::satisfyConstraints()
{
  math::Rectf bounds(particle.radius, particle.radius, windowInfo.width - particle.radius * 2, windowInfo.height - particle.radius * 2);
  auto velocity = particle.position - particle.previousPosition;

  if (particle.position.x < bounds.x1)
  {
    particle.position.x = bounds.x1 - velocity.x * 0.5f;
    particle.previousPosition.x = bounds.x1;
  }
  else if (particle.position.x > bounds.x2)
  {
    particle.position.x = bounds.x2 -  velocity.x * 0.5f;
    particle.previousPosition.x = bounds.x2;
  }

  if (particle.position.y < bounds.y1)
  {
    particle.position.y = bounds.y1 - velocity.y * 0.5f;
    particle.previousPosition.y = bounds.y1;
  }
  else if (particle.position.y > bounds.y2)
  {
    particle.position.y = bounds.y2 - velocity.y * 0.5f;
    particle.previousPosition.y = bounds.y2;
  }
}
