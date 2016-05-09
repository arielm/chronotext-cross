#include "Sketch.h"

#include "Triangulator.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;

Sketch::Sketch()
:
faceBatch(GL_TRIANGLES),
normalBatch(GL_LINES)
{}

void Sketch::setup()
{
  state
    .setShaderColor(1, 1, 1, 1)
    .glLineWidth(2);

  faceBatch.setShader(lambertShader);
  normalBatch.setShader(colorShader);

  // ---

  Matrix matrix;
  matrix.translate(-50, -50, 0);

  Triangulator triangulator;
  triangulator
    .add(Rectf(0, 0, 100, 100))
    .add(Rectf(10, 10, 80, 80))
    .process(faceBatch, matrix, glm::vec4(1, 0.5f, 0, 1));

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

  faceBatch
    .setShaderMatrix<MVP>(mvpMatrix)
    .setShaderMatrix<NORMAL>(mvMatrix.getNormalMatrix())
    .flush();

  normalBatch
    .setShaderMatrix<MVP>(mvpMatrix)
    .flush();
}
