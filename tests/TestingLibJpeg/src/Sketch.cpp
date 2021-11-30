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
    .setShader(textureShader)
    .setShaderColor(1, 1, 1, 1)
    .setTexture(textures[1]);

  draw::Sprite()
    .setAnchor(0.5f, 0.5f)
    .append(textureBatches[0], Matrix().scale(0.4f));

  draw::Sprite()
    .setAnchor(0.5f, 0)
    .append(textureBatches[1], Matrix().scale(0.167f).translate(0, 0, 25));

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  glm::mat4 projectionMatrix = glm::perspective(45 * D2R, windowInfo.aspectRatio(), 0.1f, 1000.0f);

  Matrix modelViewMatrix;
  modelViewMatrix
    .scale(1, -1, 1)
    .translate(0, 0, -300)
    .rotateX(15 * D2R)
    .rotateY(sinf(clock()->getTime() * 2.0f) * 0.25f);

  State()
    .setShaderMatrix(modelViewMatrix * projectionMatrix)
    .apply();

  textureBatches[0].flush();
  textureBatches[1].flush();
}

void Sketch::loadTextures()
{
  textures[0] = Texture(Texture::ImageRequest("Louis.jpg")
    .setAnisotropy(true));

  textures[1] = Texture(Texture::MaskedRequest("Gabrielle.jpg", "Gabrielle_mask.png")
    .setFlags(image::FLAGS_RBGA, image::FLAGS_TRANSLUCENT)
    .setAnisotropy(true));
}
