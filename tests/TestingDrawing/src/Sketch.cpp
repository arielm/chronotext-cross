#include "Sketch.h"

#include "gl/draw/Texture.h"
#include "gl/draw/Rect.h"
#include "gl/draw/Circle.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;

void Sketch::setup()
{
  initTextures();

  // ---

  glm::mat4 projectionMatrix = glm::ortho(0.0f, windowInfo.width, 0.0f, windowInfo.height);

  Matrix modelViewMatrix;
  modelViewMatrix
    .translate(windowInfo.size / 2.0f)
    .scale(1, -1);

  state.setShaderMatrix(modelViewMatrix * projectionMatrix);

  // ---

  textureBatch
    .setShader(textureAlphaShader)
    .setTexture(texture);

  colorBatch
    .setShader(colorShader);

  // ---

  Matrix matrix;

  matrix.push()
    .translate(200, 100)
    .rotateZ(30 * D2R);
  draw::Rect()
    .setColor(1, 1, 0.5f, 1)
    .fill(colorBatch, matrix, Rectf(-200, -150, 300, 150));
  matrix.pop();

  draw::Circle()
    .setColor(1, 0.5f, 0, 1)
    .setRadius(100)
    .fill(colorBatch, matrix);

  matrix.push()
    .scale(0.5f)
    .rotateZ(-15 * D2R);
  draw::Texture()
    .setColor(1, 1, 1, 1)
    .fillFromCenter(textureBatch, matrix, 0, 0);
  matrix.pop();

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  state.apply();
  colorBatch.flush(state);
  textureBatch.flush(state);
}

void Sketch::initTextures()
{
  texture = Texture(Texture::Request("lys_32.png")
    .setFlags(image::FLAGS_TRANSLUCENT_INVERSE)
    .setMipmap(true)
    .setWrap(GL_REPEAT, GL_REPEAT));
}
