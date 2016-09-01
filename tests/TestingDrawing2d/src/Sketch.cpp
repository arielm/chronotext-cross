#include "Sketch.h"

#include "chr/gl/draw/Rect.h"
#include "chr/gl/draw/Circle.h"
#include "chr/gl/Triangulator.h"
#include "chr/shape/EquilateralTriangle.h"
#include "chr/shape/FivePointedStar.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;

Sketch::Sketch()
:
strokeBatch(GL_LINES)
{}

void Sketch::setup()
{
  initTextures();

  // ---

  glm::mat4 projectionMatrix = glm::ortho(0.0f, windowInfo.width, 0.0f, windowInfo.height);

  Matrix modelViewMatrix;
  modelViewMatrix
    .translate(windowInfo.size / 2.0f)
    .scale(1, -1);

  // ---

  state
    .setShaderMatrix(modelViewMatrix * projectionMatrix)
    .glLineWidth(2);

  textureBatch
    .setShader(textureAlphaShader)
    .setTexture(texture);

  backgroundBatch
    .setShader(colorShader);

  foregroundBatch
    .setShader(colorShader);

  strokeBatch
    .setShader(colorShader)
    .setShaderColor(1, 1, 1, 1);

  // ---

  Matrix matrix;

  matrix.push()
    .translate(200, 100)
    .rotateZ(30 * D2R);
  textureMatrix.set(matrix);

  draw::Rect()
    .setColor(1, 1, 0.5f, 1)
    .setBounds(-200, -150, 300, 150)
    .append(backgroundBatch, matrix);
  matrix.pop();

  draw::Circle()
    .setColor(1, 0.5f, 0, 1)
    .setRadius(100)
    .append(foregroundBatch, matrix);

  Triangulator triangulator1;
  triangulator1
    .setContourCapture(Triangulator::CAPTURE_FRONT)
    .setColor(1, 0.25f, 0.25f, 1)
    .add(shape::FivePointedStar().setOuterRadius(100).append())
    .fill(foregroundBatch, matrix);

  triangulator1.exportContours(strokeBatch, matrix);

  matrix
    .translate(-200, 50)
    .rotateZ(30 * D2R);

  Triangulator triangulator2;
  triangulator2
    .setContourCapture(Triangulator::CAPTURE_FRONT)
    .setColor(0.25f, 1, 0, 1)
    .add(shape::EquilateralTriangle().setSideLength(150).append())
    .add(shape::EquilateralTriangle().setSideLength(120).append())
    .fill(foregroundBatch, matrix);

  triangulator2.exportContours(strokeBatch, matrix);

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

  textureBatch.clear();

  draw::Rect()
    .setColor(0, 0, 0, 0.667f)
    .setBounds(-200, -150, 300, 150)
    .setTextureScale(0.5f)
    .setTextureOffset(0, clock()->getTime() * 20)
    .append(textureBatch, textureMatrix);

  // ---

  state.apply();

  backgroundBatch.flush();
  textureBatch.flush();
  foregroundBatch.flush();
  strokeBatch.flush();
}

void Sketch::initTextures()
{
  texture = Texture(Texture::Request("lys_32.png")
    .setFlags(image::FLAGS_TRANSLUCENT_INVERSE)
    .setMipmap(true)
    .setWrap(GL_REPEAT, GL_REPEAT));
}
