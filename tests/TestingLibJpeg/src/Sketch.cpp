#include "Sketch.h"

#include "chr/gl/draw/Sprite.h"
#include "chr/gl/draw/Rect.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;

void Sketch::setup()
{
  initTextures();

  // ---

  textureBatches[0].setShader(textureShader);
  textureBatches[0].setShaderColor(1, 1, 1, 1);
  textureBatches[0].setTexture(textures[0]);

  textureBatches[1].setShader(textureAlphaShader);
  textureBatches[1].setShaderColor(1, 0.5f, 0, 1);
  textureBatches[1].setTexture(textures[1]);

  textureBatches[2].setShader(textureShader);
  textureBatches[2].setShaderColor(1, 1, 1, 1);
  textureBatches[2].setTexture(textures[2]);

  // ---

  Matrix matrix;

  matrix
    .push()
    .scale(0.333f);
  draw::Sprite()
    .setAnchor(0.5f, 0.5f)
    .append(textureBatches[0], matrix);

  matrix
    .pop()
    .translate(0, 0, 5);
  draw::Rect()
    .setBounds(-200, -150, 400, 300)
    .append(textureBatches[1], matrix);

  matrix
    .translate(0, 0, 5)
    .scale(0.75f);
  draw::Sprite()
    .setAnchor(0.5f, 0.5f)
    .append(textureBatches[2], matrix, 100, 100);

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

  glm::mat4 projectionMatrix = glm::perspective(60 * D2R, windowInfo.width / windowInfo.height, 0.1f, 1000.0f);

  Matrix modelViewMatrix;
  modelViewMatrix
    .scale(1, -1, 1)
    .translate(0, 0, -300)
    .rotateX(15 * D2R)
    .rotateY(clock()->getTime());

  // ---

  State()
    .setShaderMatrix(modelViewMatrix * projectionMatrix)
    .apply();

  textureBatches[0].flush();
  textureBatches[1].flush();
  textureBatches[2].flush();
}

void Sketch::initTextures()
{
  textures[0] = Texture(Texture::MaskedRequest("6980491_UN1_800.jpg", "6980491_UN1_800_MASK.png")
    .setFlags(image::FLAGS_RBGA | image::FLAGS_POT, image::FLAGS_TRANSLUCENT)
    .setAnisotropy(true));

  textures[1] = Texture(Texture::ImageRequest("camo.jpg")
    .setFlags(image::FLAGS_TRANSLUCENT)
    .setMipmap(true)
    .setAnisotropy(true)
    .setWrap(GL_REPEAT, GL_REPEAT));

  textures[2] = Texture(Texture::ImageRequest("banski.jpg")
    .setAnisotropy(true));
}
