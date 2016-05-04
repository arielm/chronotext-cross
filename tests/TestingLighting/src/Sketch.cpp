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
shader("SimpleLightingShader.vert", "SimpleLightingShader.frag")
{}

void Sketch::setup()
{
  state
    .setShaderColor(1, 1, 1, 1)
    .glLineWidth(2);

  faceBatch.setShader(shader);
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
    .setColor(0, 0, 0, 1)
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
    normalBatch.addVertex(vertex.position);
    normalBatch.addVertex(vertex.position + vertex.normal * 10.0f);
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

  glm::mat4 pMatrix = glm::perspective(60 * D2R, windowInfo.width / windowInfo.height, 0.1f, 1000.0f);

  Matrix mvMatrix;
  mvMatrix
    .scale(1, -1, 1)
    .translate(0, 0, -300)
    .rotateX(20 * D2R)
    .rotateY(clock()->getTime())
    .rotateZ(clock()->getTime() * 0.25f);

  // ---

  faceBatch
    .setShaderUniform("u_n_matrix", mvMatrix.getNormalMatrix())
    .setShaderUniform("u_ambient_color", 0.3f, 0.3f, 0.3f)
    .setShaderUniform("u_diffuse_color", 1.0f, 1.0f, 1.0f)
    .setShaderUniform("u_light_dir", 0.0f, 0.0f, 1.0f);

  state
    .setShaderMatrix(mvMatrix * pMatrix)
    .apply();

  faceBatch.flush(state);
  normalBatch.flush(state);
}