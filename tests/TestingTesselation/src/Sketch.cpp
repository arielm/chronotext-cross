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

  flatBatch.setShader(colorShader);
  strokeBatch.setShader(colorShader);
  normalBatch.setShader(colorShader);

  lightenBatch.setTexture(texture);

  // ---

  Matrix matrix;
  MatrixAffine affine;

  matrix.translate(0, 0, 75);

  auto rect = shape::Rect().setSize(100, 100);
  Shape polygons1;
  polygons1
    .addPath(rect.append())
    .addPath(
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
    .extrude(lightenBatch, matrix, -150);

  triangulator1.exportContours(strokeBatch, matrix);

  //

  matrix
    .setIdentity()
    .translate(0, 0, -25);

  affine
    .setIdentity()
    .translate(100, 0)
    .rotate(90 * D2R);

  auto triangle = shape::EquilateralTriangle().setSideLength(100);
  Shape polygons2;
  polygons2
    .addPath(
      affine
        .transformPoints(triangle.append()))
    .addPath(
      affine
        .scale(0.5f)
        .transformPoints(triangle.append()));

  Triangulator triangulator2;
  triangulator2
    .setContourCapture(Triangulator::CAPTURE_ALL)
    .setColor(0.25f, 0.75f, 0.0f, 1.0f)
    .add(polygons2)
    .setTextureScale(0.125f)
    .extrude(flatBatch, matrix, 50);

  triangulator2.exportContours(strokeBatch, matrix);

  //

  matrix
    .rotateX(90 * D2R)
    .translate(0, 0, 150);

  affine.setIdentity();

  auto circle = shape::Circle().setRadius(50);
  Shape polygons3;
  polygons3
    .addPath(
      affine
        .transformPoints(circle.append()))
    .addPath(
      affine
        .scale(0.8f)
        .transformPoints(circle.append()));

  Triangulator triangulator3;
  triangulator3
    .setColor(1.0f, 0.0f, 0.0f, 1.0f)
    .add(polygons3)
    .setTextureScale(0.125f)
    .extrude(lightenBatch, matrix, -75);

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

  auto projectionMatrix = glm::perspective(60 * D2R, windowInfo.width / windowInfo.height, 0.1f, 1000.0f);

  Matrix mvMatrix;
  mvMatrix
    .scale(1, -1, 1)
    .translate(0, 0, -300)
    .rotateY(clock()->getTime())
    .rotateZ(clock()->getTime() * 0.25f);

  auto mvpMatrix = mvMatrix * projectionMatrix;

  // ---

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2, 1);

  state.apply();

  lightenBatch
    .setShader(lambertShader)
    .setShaderMatrix<MVP>(mvpMatrix)
    .setShaderMatrix<NORMAL>(mvMatrix.getNormalMatrix())
    .flush();

  flatBatch
    .setShaderMatrix<MVP>(mvpMatrix)
    .flush();

  glDepthMask(GL_FALSE);
  glDisable(GL_POLYGON_OFFSET_FILL);

  lightenBatch
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
  texture = Texture(
    Texture::Request("lys_32.png")
      .setFlags(image::FLAGS_TRANSLUCENT_INVERSE)
      .setMipmap(true)
      .setWrap(GL_REPEAT, GL_REPEAT)
      .setAnisotropy(true));
}
