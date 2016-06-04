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
using namespace path;

Sketch::Sketch()
:
contourBatch(GL_LINES),
normalBatch(GL_LINES)
{}

void Sketch::setup()
{
  camera
    .setFov(60)
    .setClip(0.1f, 1000.0f)
    .setWindowSize(windowInfo.size);

  state
    .setShaderColor(1, 1, 1, 1)
    .glLineWidth(2);

  contourBatch.setShader(colorShader);
  normalBatch.setShader(colorShader);
  lightenBatch.setShader(lambertShader);

  // ---

  Matrix matrix;
  MatrixAffine affine;

  matrix.translate(0, 0, 75);

  auto rect = shape::Rect().setSize(100, 100);
  Shape polygons;
  polygons
    .addPath(rect.append())
    .addPath(
      affine
        .scale(0.75f)
        .rotate(15 * D2R)
        .transformPoints(rect.append()));

  Triangulator triangulator;
  triangulator
    .setContourCapture(Triangulator::CAPTURE_ALL)
    .setColor(1.0f, 0.5f, 0.0f, 1.0f)
    .add(polygons)
    .setTextureScale(0.125f)
    .extrude(lightenBatch, matrix, -150);

  triangulator.exportContours(contourBatch, matrix);

  // ---

  for (auto &vertex : lightenBatch.vertexBuffer->storage)
  {
    normalBatch.addVertices(vertex.position, vertex.position + vertex.normal * 5.0f);
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

  camera.getMVMatrix()
    .setIdentity()
    .scale(1, -1, 1)
    .translate(0, 0, -300)
    .rotateY(clock()->getTime())
    .rotateZ(clock()->getTime() * 0.25f * 0);

  auto eyep = camera.getEyePosition();
  auto ray = camera.getRay(glm::vec2(400, 300));

  LOGI << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << endl;
//  LOGI << eyep.x << " " << eyep.y << " " << eyep.z << endl;

  // ---

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2, 1);

  state
    .setShaderMatrix<MVP>(camera.getMVPMatrix())
    .setShaderMatrix<NORMAL>(camera.getNormalMatrix())
    .apply();

  lightenBatch.flush();

  glDepthMask(GL_FALSE);
  glDisable(GL_POLYGON_OFFSET_FILL);

  contourBatch.flush();
  normalBatch.flush();
}
