#include "Sketch.h"

#include "chr/gl/draw/Sprite.h"

using namespace std;
using namespace chr;
using namespace gl;

void Sketch::setup()
{
  loadTextures();

  textureBatches[0]
    .setShader(textureShader)
    .setShaderColor(1, 1, 1, 1)
    .setTexture(textures[0]);

  textureBatches[1]
    .setShader(textureAlphaShader)
    .setShaderColor(1, 1, 1, 1)
    .setTexture(textures[1]);

  draw::Sprite()
    .setAnchor(0.5f, 0.5f)
    .append(textureBatches[0]);

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  glm::mat4 projectionMatrix = glm::ortho(0.0f, windowInfo.width, 0.0f, windowInfo.height);

  Matrix modelViewMatrix;
  modelViewMatrix
    .translate(windowInfo.center())
    .scale(1, -1);

  State()
    .setShaderMatrix(modelViewMatrix * projectionMatrix)
    .apply();

  textureBatches[0].flush();

  textureBatches[1].clear();
  draw::Sprite()
    .setAnchor(0.5f, 0)
    .append(textureBatches[1], Matrix()
      .translate(0, sinf(clock()->getTime()) * 20)
      .scale(0.75f));
  textureBatches[1].flush();
}

void Sketch::loadTextures()
{
  textures[0] = Texture(Texture::ImageRequest("moon.png")
    .setFlags(image::FLAGS_RBGA)
    .setMipmap(true));

  textures[1] = Texture(Texture::ImageRequest("Banksy floating balloon girl.png")
    .setFlags(image::FLAGS_TRANSLUCENT));
}
