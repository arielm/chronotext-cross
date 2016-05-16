#include "Sketch.h"

#include "gl/draw/Cube.h"

using namespace std;
using namespace chr;
using namespace gl;

Sketch::Sketch()
:
xyzBatch(GL_TRIANGLES)
{}

void Sketch::setup()
{
  xyzBatch
    .setShader(lambertShader)
    .setShaderColor(0.25f, 0.25f, 0.25f, 1);

  // ---

  Matrix matrix;

  draw::Cube()
    .setSize(100)
    .append(xyzBatch, matrix);

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

  state
    .setShaderMatrix<MVP>(mvpMatrix)
    .setShaderMatrix<NORMAL>(mvMatrix.getNormalMatrix())
    .apply();

  xyzBatch.flush();
}
