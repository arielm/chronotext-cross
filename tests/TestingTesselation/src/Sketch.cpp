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

Sketch::Sketch()
:
strokeBatch(GL_LINES),
normalBatch(GL_LINES)
{}

void Sketch::setup()
{
  initTextures();

  // ---

  state
    .setShaderColor(1, 1, 1, 1)
    .glLineWidth(2);

  strokeBatch.setShader(colorShader);
  normalBatch.setShader(colorShader);

  fillBatch
    .setTexture(texture);

  // ---

  Matrix matrix;
  MatrixAffine affine;

  matrix.translate(0, 0, 75);

  vector<vector<glm::vec2>> polygons1;
  auto rect = shape::Rect().setSize(100, 100);
  polygons1
    .emplace_back(rect.append());
  polygons1
    .emplace_back(
      affine
        .scale(0.75f)
        .rotate(15 * D2R)
        .transformPoints(rect.append()));

  Triangulator triangulator1;
  triangulator1
    .setContourCapture(Triangulator::CAPTURE_ALL)
    .setColor(1.0f, 0.5f, 0.0f, 1.0f)
    .add(polygons1)
    .setTextureScale(0.125f)
    .extrude(fillBatch, matrix, -150);

  triangulator1.exportContours(strokeBatch, matrix);

  //

  matrix
    .setIdentity()
    .translate(0, 0, -25);

  affine
    .setIdentity()
    .translate(100, 0)
    .rotate(90 * D2R);

  vector<vector<glm::vec2>> polygons2;
  auto triangle = shape::EquilateralTriangle().setSideLength(100);
  polygons2
    .emplace_back(
      affine
        .transformPoints(triangle.append()));
  polygons2
    .emplace_back(
      affine
        .scale(0.5f)
        .transformPoints(triangle.append()));

  Triangulator triangulator2;
  triangulator2
    .setFrontFace(GL_CW)
    .setColor(0.5f, 1.0f, 0.0f, 1.0f)
    .add(polygons2)
    .setTextureScale(0.125f)
    .stamp(fillBatch, matrix);

  //

  matrix
    .rotateX(90 * D2R)
    .translate(0, 0, 150);

  affine.setIdentity();

  vector<vector<glm::vec2>> polygons3;
  auto circle = shape::Circle().setRadius(50);
  polygons3
    .emplace_back(
      affine
        .transformPoints(circle.append()));
  polygons3
    .emplace_back(
      affine
        .scale(0.8f)
        .transformPoints(circle.append()));

  Triangulator triangulator3;
  triangulator3
    .setColor(1.0f, 0.0f, 0.0f, 1.0f)
    .add(polygons3)
    .setTextureScale(0.125f)
    .extrude(fillBatch, matrix, -75);

  // ---

  for (auto &vertex : fillBatch.vertexBuffer->storage)
  {
    normalBatch
      .addVertex(vertex.position)
      .addVertex(vertex.position + vertex.normal * 5.0f);
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

  glm::mat4 projectionMatrix = glm::perspective(60 * D2R, windowInfo.width / windowInfo.height, 0.1f, 1000.0f);

  Matrix mvMatrix;
  mvMatrix
    .scale(1, -1, 1)
    .translate(0, 0, -300)
    .rotateY(clock()->getTime())
    .rotateZ(clock()->getTime() * 0.25f);

  auto mvpMatrix = mvMatrix * projectionMatrix;

  // ---

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(4, 1);

  state.apply();

  fillBatch
    .setShader(lambertShader)
    .setShaderMatrix<MVP>(mvpMatrix)
    .setShaderMatrix<NORMAL>(mvMatrix.getNormalMatrix())
    .flush();

  glDepthMask(GL_FALSE);
  glDisable(GL_POLYGON_OFFSET_FILL);

  fillBatch
    .setShader(textureAlphaShader)
    .flush();

  strokeBatch
    .setShaderMatrix<MVP>(mvpMatrix)
    .flush();

  normalBatch
    .setShaderMatrix<MVP>(mvpMatrix)
    .flush();
}

void Sketch::initTextures()
{
  texture = Texture(Texture::Request("lys_32.png")
    .setFlags(image::FLAGS_TRANSLUCENT_INVERSE)
    .setMipmap(true).setWrap(GL_REPEAT, GL_REPEAT)
    .setAnisotropy(true));
}
