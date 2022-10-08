#include "Sketch.h"

#include "chr/gl/draw/Sphere.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace draw;

static constexpr float SUN_EARTH_DISTANCE = 200;
static constexpr float EARTH_MOON_DISTANCE = 27;

void Sketch::setup()
{
  sunBatch
    .setShader(lambertShader)
    .setShaderColor(1, 1, 0.5f, 1);

  earthBatch
    .setShader(lambertShader)
    .setShaderColor(0.5f, 0.67f, 1, 1);

  moonBatch
    .setShader(lambertShader)
    .setShaderColor(0.75f, 0.75f, 0.75f, 1);

  Sphere()
    .setFrontFace(CW)
    .setRadius(45)
    .setSectorCount(60)
    .setStackCount(30)
    .append(sunBatch, Matrix());

  pathBatch
    .setPrimitive(GL_LINE_STRIP)
    .setShader(colorShader)
    .setShaderColor(1, 1, 1, 0.25f);

  createPath();

  // ---

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(0.125f, 0.125f, 0.125f, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ---

  auto projectionMatrix = glm::perspective(60 * D2R, windowInfo.aspectRatio(), 0.1f, 1000.0f);

  Matrix viewMatrix;
  viewMatrix
    .translate(0, 0, -400)
    .rotateX(22 * D2R);

  State()
    .setShaderMatrix<MVP>(viewMatrix * projectionMatrix)
    .setShaderMatrix<NORMAL>(viewMatrix.getNormalMatrix())
    .apply();

  sunBatch.flush();

  Matrix matrix;
  float t = clock()->getTime();

  matrix
    .rotateY(t * 0.5f)
    .translate(0, 0, SUN_EARTH_DISTANCE);

  earthBatch.clear();
  Sphere()
    .setFrontFace(CW)
    .setRadius(20)
    .setSectorCount(40)
    .setStackCount(20)
    .append(earthBatch, matrix);
  earthBatch.flush();

  matrix
    .rotateY(t * 2.0f)
    .translate(0, 0, EARTH_MOON_DISTANCE);

  moonBatch.clear();
  Sphere()
    .setFrontFace(CW)
    .setRadius(5)
    .setSectorCount(30)
    .setStackCount(15)
    .append(moonBatch, matrix);
  moonBatch.flush();

  pathBatch.flush();
}

void Sketch::createPath()
{
  float radius = SUN_EARTH_DISTANCE;
  float segmentLength = 10;
  int n = ceilf(TWO_PI * radius / segmentLength) + 1;

  for (int i = 0; i < n; i++)
  {
    float d = i * segmentLength / radius;
    pathBatch.addVertex(sinf(-d) * radius, 0, -cosf(-d) * radius);
  }
}
