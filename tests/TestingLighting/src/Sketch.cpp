#include "Sketch.h"

#include "gl/draw/Texture.h"
#include "gl/draw/Rect.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;

void Sketch::setup()
{
  colorBatch.setShader(colorShader);

  // ---

  Matrix matrix;
  matrix
    .translate(50, 100)
    .rotateZ(30 * D2R);
  draw::Rect()
    .setColor(1, 1, 0.5f, 1)
    .fill(colorBatch, matrix, Rectf(-100, -100, +100, +100));

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

  glm::mat4 projectionMatrix = glm::perspective(60 * D2R, windowInfo.width / windowInfo.height, 0.1f, 1000.0f);

  Matrix modelViewMatrix;
  modelViewMatrix
    .scale(1, -1, 1)
    .translate(0, 0, -300)
    .rotateX(15 * D2R)
    .rotateY(getElapsedSeconds());

  state
    .setShaderMatrix(modelViewMatrix * projectionMatrix)
    .apply();

  colorBatch.flush(state);
}
