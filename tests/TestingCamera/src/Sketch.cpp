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

  state.glLineWidth(2);

  contourBatch
    .setShader(colorShader)
    .setShaderColor(1, 1, 1, 0.5f);

  normalBatch
    .setShader(colorShader)
    .setShaderColor(1, 1, 1, 1);

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
    .setFrontFace(CCW)
    .extrude(lightenBatch, matrix, -150);

  triangulator.exportContours(contourBatch, matrix);

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
    .rotateZ(clock()->getTime() * 0.25f);

  if (mousePressed)
  {
    processRay(mousePosition, camera.getMVMatrix(), CCW);
  }

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

void Sketch::addTouch(int index, float x, float y)
{
  mousePressed = true;
  mousePosition = { x, y };
}

void Sketch::updateTouch(int index, float x, float y)
{
  mousePosition = { x, y };
}

void Sketch::removeTouch(int index, float x, float y)
{
  mousePressed = false;
}

void Sketch::processRay(const glm::vec2 &position, const Matrix &matrix, int frontFace)
{
  auto ray = camera.getRay(position);

  auto &vertices = lightenBatch.vertices();
  auto &indices = lightenBatch.indices();
  auto indexCount = indices.size();

  float bestT = numeric_limits<float>::max(); // XXX
  int bestIndex = -1;

  for (auto i = 0; i < indexCount; i += 3)
  {
    const auto &v0 = vertices[indices[i]].position;
    const auto &v1 = vertices[indices[i + 1]].position;
    const auto &v2 = vertices[indices[i + 2]].position;

    float t = ray.triangleIntersection(v0, v1, v2, true, frontFace);

    if (t > 0)
    {
      if (bestT > t) // XXX
      {
        bestT = t;
        bestIndex = i;
      }
    }
  }

  if (bestIndex != -1)
  {
    glm::vec3 pickedPoint(ray.origin + ray.direction * bestT);

    const auto &v0 = vertices[indices[bestIndex]].position;
    const auto &v1 = vertices[indices[bestIndex + 1]].position;
    const auto &v2 = vertices[indices[bestIndex + 2]].position;

    glm::vec3 pickedNormal(glm::normalize(glm::cross(v1 - v0, v2 - v0)));

    normalBatch.addVertices(pickedPoint, pickedPoint + pickedNormal * 10.0f * (frontFace == CW ? +1.0f : -1.0f));
    normalBatch.vertexBuffer.requestUpload();
  }
}
