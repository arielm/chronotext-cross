#include "Sketch.h"

#include "gl/draw/Texture.h"
#include "gl/draw/Rect.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;

Sketch::Sketch()
:
faceBatch(GL_TRIANGLES),
normalBatch(GL_LINES),
phongShader("PhongShader.vert", "PhongShader.frag")
{}

void Sketch::setup()
{
  state
    .setShaderColor(1, 1, 1, 1)
    .glLineWidth(2);

  normalBatch.setShader(colorShader);

  // ---

  Matrix matrix;

  matrix
    .translate(0, 0, 50)
    .translate(-50, -50, 0)
    .push();
  draw::Rect()
    .setColor(0.75f, 0.75f, 0.75f, 1)
    .fill(faceBatch, matrix, Rectf(0, 0, 100, 100));

  matrix
    .translate(100, 0, 0)
    .rotateY(90 * D2R);
  draw::Rect()
    .setColor(1, 0.5f, 0, 1)
    .fill(faceBatch, matrix, Rectf(0, 0, 100, 100));

  matrix
    .translate(100, 0, 0)
    .rotateY(90 * D2R);
  draw::Rect()
    .setColor(1, 0, 0, 1)
    .fill(faceBatch, matrix, Rectf(0, 0, 100, 100));

  matrix
    .translate(100, 0, 0)
    .rotateY(90 * D2R);
  draw::Rect()
    .setColor(1, 1, 0, 1)
    .fill(faceBatch, matrix, Rectf(0, 0, 100, 100));

  matrix
    .translate(0, 100, 0)
    .rotateX(-90 * D2R);
  draw::Rect()
    .setColor(0.5f, 1.0f, 0.5f, 1)
    .fill(faceBatch, matrix, Rectf(0, 0, 100, 100));

  matrix
    .pop()
    .rotateX(-90 * D2R);
  draw::Rect()
    .setColor(0.25f, 0.25f, 0.25f, 1)
    .fill<GL_CW>(faceBatch, matrix, Rectf(0, 0, 100, 100));

  // ---

  for (auto &vertex : faceBatch.vertexBuffer->storage)
  {
    normalBatch
      .addVertex(vertex.position)
      .addVertex(vertex.position + vertex.normal * 10.0f);
  }

  // ---

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ---

  glm::mat4 projectionMatrix = glm::perspective(60 * D2R, windowInfo.width / windowInfo.height, 0.1f, 1000.0f);

  Matrix mvMatrix;
  mvMatrix
    .scale(1, -1, 1)
    .translate(0, 0, -300)
    .rotateY(clock()->getTime())
    .rotateZ(clock()->getTime() * 0.25f);

  auto mvpMatrix = mvMatrix * projectionMatrix;

  // ---

  state.apply();

  if (pressed)
  {
    faceBatch
      .setShader(phongShader)
      .setShaderMatrix<MV>(mvMatrix)
      .setShaderMatrix<PROJECTION>(projectionMatrix);
  }
  else
  {
    faceBatch
      .setShader(lambertShader)
      .setShaderMatrix<MVP>(mvpMatrix);
  }

  faceBatch
    .setShaderMatrix<NORMAL>(mvMatrix.getNormalMatrix())
    .flush();

  normalBatch
    .setShaderMatrix<MVP>(mvpMatrix)
    .flush();
}

void Sketch::addTouch(int index, float x, float y)
{
  pressed = true;
}

void Sketch::removeTouch(int index, float x, float y)
{
  pressed = false;
}