
#include "Sketch.h"
#include "gl/Matrix.h"
#include "gl/draw/Texture.h"

using namespace std;
using namespace chr;
using namespace gl;

static constexpr float DOT_RADIUS_DP = 22;
static constexpr float DOT_RADIUS_PIXELS = 56; // SPECIFIC TO "dot_112.png"

void Sketch::setup()
{
  scale = getDisplayInfo().density / DisplayInfo::REFERENCE_DENSITY;
  auto projectionMatrix = glm::ortho(0.0f, windowInfo.width, windowInfo.height, 0.0f);

  initTextures();

  textureState
    .setShader(textureAlphaShader)
    .setShaderColor(1, 1, 1, 1)
    .setShaderMatrix(projectionMatrix);

  textureBatch.setTexture(texture);

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void Sketch::draw()
{
  glClearColor(0, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  textureBatch.clear();

  for (auto it = touchPositions.begin(); it != touchPositions.end(); ++it)
  {
    drawDot(it->second, scale * DOT_RADIUS_DP);
  }

  textureState.apply();
  textureBatch.flush();

  touchPositions.clear();
}

void Sketch::addTouch(int index, float x, float y)
{
  touchPositions[index] = glm::vec2(x, y);
}

void Sketch::updateTouch(int index, float x, float y)
{
  touchPositions[index] = glm::vec2(x, y);
}

void Sketch::drawDot(const glm::vec2 &position, float radius)
{
  Matrix matrix;
  matrix
    .translate(position)
    .scale(radius / DOT_RADIUS_PIXELS);

  draw::Texture()
    .setAnchor(0.5f, 0.5f)
    .append(textureBatch, matrix);
}

void Sketch::initTextures()
{
  texture = Texture(Texture::Request("dot_112.png")
    .setFlags(image::FLAGS_TRANSLUCENT)
    .setMipmap(true));
}
