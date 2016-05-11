#include "Sketch.h"

#include "gl/Triangulator.h"
#include "math/MatrixAffine.h"
#include "shape/Rect.h"
#include "shape/EquilateralTriangle.h"
#include "shape/Circle.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;

Sketch::Sketch()
:
fillBatch(GL_TRIANGLES),
strokeBatch(GL_LINES),
normalBatch(GL_LINES)
{}

void Sketch::setup()
{
  state
    .setShaderColor(1, 1, 1, 1)
    .glLineWidth(2);

  fillBatch.setShader(lambertShader);
  strokeBatch.setShader(colorShader);
  normalBatch.setShader(colorShader);

  // ---

  MatrixAffine affine;
  Matrix matrix;

  matrix
    .push()
    .translate(0, 0, 75);

  Triangulator triangulator1;
  triangulator1
    .setContourCapture(Triangulator::CAPTURE_ALL)
    .setColor(1.0f, 0.5f, 0.0f, 1.0f)
    .add(affine.transformPoints(shape::Rect().setSize(100, 100).get()));

  affine
    .scale(0.75f)
    .rotate(15 * D2R);

  triangulator1
    .add(affine.transformPoints(shape::Rect().setSize(100, 100).get()))
    .extrude(fillBatch, matrix, -150);

  triangulator1.exportContours(strokeBatch, matrix);

  //

  matrix
    .pop()
    .translate(0, 0, -25);

  affine
    .setIdentity()
    .translate(100, 0)
    .rotate(90 * D2R);

  Triangulator triangulator2;
  triangulator2
    .setFrontFace(GL_CW)
    .setColor(0.5f, 1.0f, 0.0f, 1.0f)
    .add(affine.transformPoints(shape::EquilateralTriangle().setSideLength(100).get()));

  affine.scale(0.75f);

  triangulator2
    .add(affine.transformPoints(shape::EquilateralTriangle().setSideLength(100).get()))
    .stamp(fillBatch, matrix);

  //

  matrix
    .rotateX(90 * D2R)
    .translate(0, 0, 150);

  affine.setIdentity();

  Triangulator triangulator3;
  triangulator3
    .setColor(1.0f, 0.0f, 0.0f, 1.0f)
    .add(affine.transformPoints(shape::Circle().setRadius(50).get()));

  affine.scale(0.8f);

  triangulator3
    .add(affine.transformPoints(shape::Circle().setRadius(50).get()))
    .extrude(fillBatch, matrix, -75);

  // ---

  for (auto &vertex : fillBatch.vertexBuffer->storage)
  {
    normalBatch
      .addVertex(vertex.position)
      .addVertex(vertex.position + vertex.normal * 5.0f);
  }

  // ---

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1);
  glDepthMask(GL_TRUE);
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

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(4, 1);

  state.apply();

  fillBatch
    .setShaderMatrix<MVP>(mvpMatrix)
    .setShaderMatrix<NORMAL>(mvMatrix.getNormalMatrix())
    .flush();

  glDepthMask(GL_FALSE);
  glDisable(GL_POLYGON_OFFSET_FILL);

  strokeBatch
    .setShaderMatrix<MVP>(mvpMatrix)
    .flush();

  normalBatch
    .setShaderMatrix<MVP>(mvpMatrix)
    .flush();
}
