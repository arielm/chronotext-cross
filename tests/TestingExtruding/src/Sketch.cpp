#include "Sketch.h"

#include "chr/gl/Triangulator.h"
#include "chr/math/MatrixAffine.h"
#include "chr/shape/Rect.h"
#include "chr/shape/EquilateralTriangle.h"
#include "chr/shape/Circle.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;
using namespace path;

Sketch::Sketch()
{}

void Sketch::setup()
{
  texture = Texture(
    Texture::ImageRequest("lys.png")
      .setMipmap(true)
      .setWrap(GL_REPEAT, GL_REPEAT)
      .setAnisotropy(true));

  flatBatch.setShader(colorShader);
  
  contourBatch
    .setPrimitive(GL_LINES)
    .setShader(colorShader)
    .setShaderColor(1, 1, 1, 1);

  lightenBatch
    .setShader(textureLambertShader)
    .setTexture(texture);

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

  triangulator1.exportContours(contourBatch, matrix);

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

  triangulator2.exportContours(contourBatch, matrix);

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

  // ---

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2, 1);

  State()
    .setShaderMatrix<MVP>(mvMatrix * projectionMatrix)
    .setShaderMatrix<NORMAL>(mvMatrix.getNormalMatrix())
    .apply();

  lightenBatch.flush();
  flatBatch.flush();

  glDepthMask(GL_FALSE);
  glDisable(GL_POLYGON_OFFSET_FILL);

  contourBatch.flush();
}
