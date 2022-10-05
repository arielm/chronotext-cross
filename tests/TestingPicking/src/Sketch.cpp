#include "Sketch.h"

#include "chr/gl/Triangulator.h"
#include "chr/math/MatrixAffine.h"
#include "chr/shape/Rect.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;
using namespace path;

void Sketch::setup()
{
  contourBatch
    .setPrimitive(GL_LINES)
    .setShader(colorShader)
    .setShaderColor(1, 1, 1, 0.5f);

  normalBatch
    .setPrimitive(GL_LINES)
    .setShader(colorShader)
    .setShaderColor(1, 1, 1, 1);

  lightenBatch
    .setShader(lambertShader)
    .setShaderColor(1.0f, 0.5f, 0.0f, 1.0f);

  // ---

  MatrixAffine affine;
  Matrix matrix;

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
    .setFrontFace(CCW)
    .add(polygons)
    .extrude(lightenBatch, matrix, -150);

  triangulator.exportContours(contourBatch, matrix);

  // ---

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::resize()
{
  camera
    .setFov(60)
    .setClip(0.1f, 1000.0f)
    .setWindowSize(windowInfo.size);
}

void Sketch::draw()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1);
  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ---

  camera.getViewMatrix()
    .setIdentity()
    .scale(1, -1, 1)
    .translate(0, 0, -300)
    .rotateY(clock()->getTime())
    .rotateZ(clock()->getTime() * 0.25f);

  if (mousePressed)
  {
    processRay(mousePosition, CCW);
  }

  // ---

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2, 1);

  State()
    .setShaderMatrix<MVP>(camera.getViewProjectionMatrix())
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

void Sketch::processRay(const glm::vec2 &position, int frontFace)
{
  auto ray = camera.getRay(position);

  auto &vertices = lightenBatch.vertices();
  auto &indices = lightenBatch.indices();
  auto indexCount = indices.size();

  float bestT = FLT_MAX; // XXX
  int bestIndex = -1;

  for (auto i = 0; i < indexCount; i += 3)
  {
    const auto &v0 = vertices[indices[i]].position;
    const auto &v1 = vertices[indices[i + 1]].position;
    const auto &v2 = vertices[indices[i + 2]].position;

    auto result = ray.triangleIntersection(v0, v1, v2, true, frontFace);

    if (result.first)
    {
      if (bestT > result.second) // XXX
      {
        bestT = result.second;
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
    normalBatch.vertexBuffer().requestUpload();
  }
}
