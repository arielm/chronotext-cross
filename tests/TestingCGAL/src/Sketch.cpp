#include "Sketch.h"

#include "gl/draw/Cube.h"
#include "gl/Triangulator.h"
#include "shape/Circle.h"

#include "PolyhedronBuilder.h"
#include "BatchPolyhedron.h"

using namespace std;
using namespace chr;
using namespace gl;

static bool showTube = true;
static bool showCube = false;

Sketch::Sketch()
:
batch3(GL_TRIANGLES)
{}

void Sketch::setup()
{
  state.setShader(colorShader);

  // ---

  Matrix matrix;

  draw::Cube()
    .setFrontFace(GL_CW)
    .setSize(100)
    .append(batch1, matrix);

  matrix
    .setTranslate(37.5f, 37.5f, 37.5f)
    .rotateY(15 * D2R);

  if (showCube)
  {
    draw::Cube()
      .setFrontFace(GL_CW)
      .setSize(75)
      .append(batch2, matrix);
  }
  else if (showTube)
  {
    vector<vector<glm::vec2>> polygons;
    polygons.emplace_back(
      shape::Circle()
        .setSegmentLength(6)
        .setRadius(50)
        .append());
    polygons.emplace_back(
      shape::Circle()
        .setSegmentLength(6)
        .setRadius(40)
        .append());

    Triangulator triangulator;
    triangulator
      .setFrontFace(GL_CW)
      .add(polygons)
      .extrude(batch2, matrix, -120);
  }
  else
  {
    assert(false);
  }

  // ---

  Polyhedron P1;
  PolyhedronBuilder<HalfedgeDS> builder1(batch1.vertexBuffer->storage, batch1.indexBuffer->storage);
  P1.delegate(builder1);

  Polyhedron P2;
  PolyhedronBuilder<HalfedgeDS> builder2(batch2.vertexBuffer->storage, batch2.indexBuffer->storage);
  P2.delegate(builder2);

  LOGI << "isClosed: " << P1.is_closed() << " | isValid: " << P1.is_valid() << " | isPureTriangle: " << P1.is_pure_triangle() << endl;
  LOGI << "isClosed: " << P2.is_closed() << " | isValid: " << P2.is_valid() << " | isPureTriangle: " << P2.is_pure_triangle() << endl;

  Timer timer;

  Nef_polyhedron nef1(P1);
  Nef_polyhedron nef2(P2);

  Nef_polyhedron nef3 = nef1 + nef2;
  Polyhedron result;
  nef3.convert_to_Polyhedron(result);

  result.append(batch3);

  timer.stop();
  LOGI << timer.getSeconds() << endl;

  // ---

  glDepthMask(GL_TRUE);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

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
    .scale(1, 1, 1)
    .translate(0, 0, -300)
    .rotateY(clock()->getTime())
    .rotateZ(clock()->getTime() * 0.25f);

  auto mvpMatrix = mvMatrix * projectionMatrix;

  // ---

  state
    .setShaderMatrix<MVP>(mvpMatrix)
    .apply();

  if (pressed)
  {
    batch1
      .setShaderColor(0.25f, 0.25f, 0.25f, 1)
      .flush();

    batch2
      .setShaderColor(0.5f, 0, 0, 1)
      .flush();
  }
  else
  {
    batch3
      .setShaderColor(1, 0.75f, 0, 1)
      .flush();
  }
}

void Sketch::addTouch(int index, float x, float y)
{
  pressed = true;
}

void Sketch::removeTouch(int index, float x, float y)
{
  pressed = false;
}
