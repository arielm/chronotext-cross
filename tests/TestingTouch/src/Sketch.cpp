
#include "Sketch.h"
#include "gl/TextureAlphaShader.h"

using namespace std;
using namespace chr;
using namespace gl;

static constexpr float DOT_RADIUS_DP = 22;
static constexpr float DOT_RADIUS_PIXELS = 56; // SPECIFIC TO "dot_112.png"

void Sketch::setup()
{
  scale = getDisplayInfo().density / DisplayInfo::REFERENCE_DENSITY;
  projectionMatrix = glm::ortho(0.0f, windowInfo.size.x, windowInfo.size.y, 0.0f);

  initTextures();

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::shutdown()
{
  glDeleteTextures(1, &texture.id);
  textureBuffer.shutdown();
}

void Sketch::draw()
{
  glClearColor(0, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  textureBuffer.setShader(textureAlphaShader);
  textureBuffer.setColor(1, 1, 1, 1);

  for (auto &position : dotPositions)
  {
    drawDot(position, scale * DOT_RADIUS_DP);
  }

  dotPositions.clear();
}

void Sketch::addTouch(int index, float x, float y)
{
  dotPositions.emplace_back(x, y);
}

void Sketch::updateTouch(int index, float x, float y)
{
  dotPositions.emplace_back(x, y);
}

void Sketch::drawDot(const glm::vec2 &position, float radius)
{
  glm::mat4 modelViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0));
  modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3(radius / DOT_RADIUS_PIXELS));

  textureBuffer.setMatrix(projectionMatrix * modelViewMatrix);
  textureBuffer.drawFromCenter(texture);
}

void Sketch::initTextures()
{
  texture = loadTexture("dot_112.png", image::FLAGS_TRANSLUCENT);
}
