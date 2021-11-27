#include "Sketch.h"

#include "chr/gl/draw/Sprite.h"

using namespace std;
using namespace chr;
using namespace gl;

static constexpr float DOT_RADIUS_DP = 22;
static constexpr float DOT_RADIUS_PIXELS = 56; // SPECIFIC TO "dot_112.png"

void Sketch::setup()
{
  scale = getDisplayInfo().density / DisplayInfo::REFERENCE_DENSITY;

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
  for (auto it = touchPositions.begin(); it != touchPositions.end(); ++it)
  {
    drawDot(it->second, scale * DOT_RADIUS_DP);
  }
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
  draw::Sprite()
    .setAnchor(0.5f, 0.5f)
    .append(textureBatch, Matrix()
      .translate(position)
      .scale(radius / DOT_RADIUS_PIXELS));
}
