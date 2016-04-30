#include "Sketch.h"
#include "gl/draw/Texture.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;

void Sketch::setup()
{
  initTextures();

  // ---

  states[0].setShader(textureShader);
  states[0].setShaderColor(1, 1, 1, 1);
  states[0].setTexture(textures[0]);

  states[1].setShader(textureAlphaShader);
  states[1].setShaderColor(1, 0.5f, 0, 1);
  states[1].setTexture(textures[1]);

  states[2].setShader(textureShader);
  states[2].setShaderColor(1, 1, 1, 1);
  states[2].setTexture(textures[2]);

  Matrix matrix;

  matrix.push();
  matrix.scale(0.333f);
  draw::Texture(textures[0]).fillFromCenter(textureBatches[0], matrix);
  matrix.pop();

  matrix.translate(0, 0, 5);
  draw::Texture(textures[1]).fillRect(textureBatches[1], matrix, Rectf(-200, -150, 400, 300));

  matrix.translate(0, 0, 5);
  matrix.scale(0.75f);
  draw::Texture(textures[2]).fillFromCenter(textureBatches[2], matrix, 100, 100);

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
    .rotateY(getElapsedSeconds());

  glm::mat4 mvpMatrix = modelViewMatrix * projectionMatrix;

  // ---

  states[0].setShaderMatrix(mvpMatrix);
  states[1].setShaderMatrix(mvpMatrix);
  states[2].setShaderMatrix(mvpMatrix);

  states[0].apply();
  textureBatches[0].flush(states[0]);

  states[1].apply();
  textureBatches[1].flush(states[1]);

  states[2].apply();
  textureBatches[2].flush(states[2]);
}

void Sketch::initTextures()
{
  textures[0] = Texture(Texture::Request("6980491_UN1_800_MASK.png")
    .setFlags(image::FLAGS_POT)
    .setAnisotropy(true));

  textures[1] = Texture(Texture::Request("lys_32.png")
    .setFlags(image::FLAGS_TRANSLUCENT_INVERSE)
    .setMipmap(true)
    .setAnisotropy(true)
    .setWrap(GL_REPEAT, GL_REPEAT));

  textures[2] = Texture(Texture::Request("expo67.png")
    .setAnisotropy(true));
}
