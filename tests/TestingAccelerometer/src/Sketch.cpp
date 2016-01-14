
#include "Sketch.h"
#include "math/Rect.h"
#include "gl/TextureAlphaShader.h"

using namespace std;
using namespace chr;
using namespace gl;

static constexpr float DOT_RADIUS_DP = 22;
static constexpr float DOT_RADIUS_PIXELS = 56; // SPECIFIC TO "dot_112.png"
static constexpr float TEXTURE_RADIUS_PIXELS = 64; // SPECIFIC TO "dot_112.png"

static constexpr float FRICTION = 0.01f;
static constexpr float DT = 1.0f;

void Sketch::setup()
{
  scale = getDisplayInfo().density / DisplayInfo::REFERENCE_DENSITY;
  projectionMatrix = glm::ortho(0.0f, windowInfo.size.x, windowInfo.size.y, 0.0f);
  particle = Particle(windowInfo.size * 0.5f, scale * DOT_RADIUS_DP);

  // ---

  initBuffers();
  initTextures();

  // ---

  glEnable(GL_TEXTURE_2D);

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::shutdown()
{
  glUseProgram(0);
  textureAlphaShader.unload();

  glDisableVertexAttribArray(textureAlphaShader.positionLocation);
  glDisableVertexAttribArray(textureAlphaShader.coordLocation);

  glDeleteBuffers(3, vboIds);
  glDeleteTextures(1, &textureInfo.id);
}

void Sketch::start(StartReason reason)
{
  acceleration = glm::zero<glm::vec2>();
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

  textureAlphaShader.use();
  {
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glEnableVertexAttribArray(textureAlphaShader.positionLocation);
    glVertexAttribPointer(textureAlphaShader.positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
    glEnableVertexAttribArray(textureAlphaShader.coordLocation);
    glVertexAttribPointer(textureAlphaShader.coordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

    GLfloat color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    glVertexAttrib4fv(textureAlphaShader.colorLocation, color);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(textureAlphaShader.samplerLocation, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[2]);

    // ---

    drawDot(particle.position, particle.radius);
  }
}

void Sketch::accelerated(AccelEvent event)
{
  acceleration = glm::vec2(+event.getRawData().x, -event.getRawData().y);
}

void Sketch::initBuffers()
{
  const GLfloat vertices[] =
  {
    -TEXTURE_RADIUS_PIXELS, +TEXTURE_RADIUS_PIXELS, 0, // A
    -TEXTURE_RADIUS_PIXELS, -TEXTURE_RADIUS_PIXELS, 0, // B
    +TEXTURE_RADIUS_PIXELS, -TEXTURE_RADIUS_PIXELS, 0, // C
    +TEXTURE_RADIUS_PIXELS, +TEXTURE_RADIUS_PIXELS, 0, // D
  };

  const GLfloat coords[] =
  {
    0, 0, // A
    0, 1, // B
    1, 1, // C
    1, 0, // D
  };

  const GLushort indices[] =
  {
    0, 1, 2,
    2, 3, 0
  };

  glGenBuffers(3, vboIds);

  glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 3, vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 3, coords, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indices, GL_STATIC_DRAW);
}

void Sketch::initTextures()
{
  textureInfo = loadTexture("dot_112.png", true);
}

void Sketch::drawDot(const glm::vec2 &position, float radius)
{
  glm::mat4 modelViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0));
  modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3(radius / DOT_RADIUS_PIXELS));

  glm::mat4 mvp = projectionMatrix * modelViewMatrix;
  glUniformMatrix4fv(textureAlphaShader.matrixLocation, 1, GL_FALSE, &mvp[0][0]);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
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
  math::Rectf bounds(particle.radius, particle.radius, windowInfo.size.x - particle.radius * 2, windowInfo.size.y - particle.radius * 2);
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
