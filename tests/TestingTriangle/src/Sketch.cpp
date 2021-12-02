#include "Sketch.h"

using namespace std;
using namespace chr;
using namespace gl;

void Sketch::setup()
{
  batch.setShader(colorShader);

  batch
    .addVertex(0, -100, glm::vec4(0, 0, 1, 1))
    .addVertex(-120, 100, glm::vec4(1, 0, 0, 1))
    .addVertex(120, 100, glm::vec4(0, 1, 0, 1));

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  glm::mat4 projectionMatrix = glm::ortho(0.0f, windowInfo.width, 0.0f, windowInfo.height);

  Matrix viewMatrix;
  viewMatrix
    .translate(windowInfo.center())
    .scale(1, -1);

  State()
    .setShaderMatrix<MVP>(viewMatrix * projectionMatrix)
    .apply();

  batch.flush();
}
