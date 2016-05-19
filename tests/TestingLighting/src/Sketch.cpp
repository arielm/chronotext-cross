#include "Sketch.h"

#include "gl/draw/Cube.h"
#include "gl/Triangulator.h"
#include "shape/Circle.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;

static bool showNormals = true;
static bool showTube = false;
static bool showCube = true;

Sketch::Sketch()
:
fillBatch(GL_TRIANGLES),
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

  if (showTube)
  {
    vector<vector<glm::vec2>> polygons;
    polygons.emplace_back(shape::Circle().setRadius(50).append());
    polygons.emplace_back(shape::Circle().setRadius(40).append());

    matrix.translate(0, 0, 50);

    Triangulator triangulator;
    triangulator
      .setColor(1.0f, 0.0f, 0.0f, 1.0f)
      .add(polygons)
      .extrude(fillBatch, matrix, -100);
  }
  else if (showCube)
  {
    draw::Cube()
      .setSize(100)
      .setColors(
        glm::vec4(0.75f, 0.75f, 0.75f, 1),
        glm::vec4(1, 0, 0, 1),
        glm::vec4(0.5f, 1.0f, 0.5f, 1),
        glm::vec4(0.25f, 0.25f, 0.25f, 1),
        glm::vec4(1, 0.5f, 0, 1),
        glm::vec4(1, 1, 0, 1))
      .append(fillBatch, matrix);
  }

  // ---

  if (showNormals)
  {
    for (auto &vertex : fillBatch.vertexBuffer->storage)
    {
      normalBatch
        .addVertex(vertex.position)
        .addVertex(vertex.position + vertex.normal * 5.0f);
    }
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
    fillBatch
      .setShader(phongShader)
      .setShaderMatrix<MV>(mvMatrix)
      .setShaderMatrix<PROJECTION>(projectionMatrix);
  }
  else
  {
    fillBatch
      .setShader(lambertShader)
      .setShaderMatrix<MVP>(mvpMatrix);
  }

  fillBatch
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