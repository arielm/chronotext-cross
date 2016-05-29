#include "Sketch.h"

#include "math/MatrixAffine.h"
#include "gl/draw/Sprite.h"
#include "gl/draw/Rect.h"
#include "gl/Triangulator.h"
#include "shape/Rect.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;
using namespace path;

static constexpr float DOT_RADIUS_PIXELS = 56; // SPECIFIC TO "dot_112.png"

Sketch::Sketch()
:
lineBatch(GL_LINE_STRIP)
{}

void Sketch::setup()
{
  initTextures();

  glm::mat4 projectionMatrix = glm::ortho(0.0f, windowInfo.width, windowInfo.height, 0.0f);

  lineState
    .setShader(colorShader)
    .setShaderColor(0, 0, 0, 0.67f)
    .setShaderMatrix(projectionMatrix);

  dotState
    .setShader(textureAlphaShader)
    .setShaderColor(1, 0, 0, 0.85f)
    .setShaderMatrix(projectionMatrix);

  flatState
    .setShader(colorShader)
    .setShaderMatrix(projectionMatrix);

  dotBatch.setTexture(dotTexture);

  // ---

  splinePath
    .setType(SplinePath::TYPE_CATMULL_ROM)
    .setSamplingTolerance(16)
    .add(200, 300)
    .add(350, 325)
    .add(450, 150)
    .add(500, 175)
    .add(550, 150);

  path
    .setMode(FollowablePath2D::MODE_MODULO)
    .add(splinePath.getPolyline());

  drawPolyline(splinePath.getPolyline());

  for (auto &point : splinePath.getPoints())
  {
    drawDot(point.x, point.y, 3);
  }

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::shutdown()
{}

void Sketch::draw()
{
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  lineState.apply();
  lineBatch.flush();

  dotState.apply();
  dotBatch.flush();

  // ---

  flatBatch.clear();

  Matrix matrix1;
  MatrixAffine affine;

  path
    .offsetToValue(clock()->getTime() * 50, 20)
    .applyToMatrix(matrix1);

  path
    .offsetToValue(clock()->getTime() * 30 + 100, 10)
    .applyToMatrix(affine);

  draw::Rect()
    .setColor(0, 0.75f, 0.25f, 0.75f)
    .setBounds(-10, -10, 20, 20)
    .append(flatBatch, matrix1);

  Triangulator triangulator;
  triangulator
    .setColor(1, 0.5f, 0.25f, 1)
    .add(affine.transformPoints(shape::Rect().setSize(10, 10).append()))
    .fill(flatBatch);

  flatState.apply();
  flatBatch.flush();
}

void Sketch::drawPolyline(const vector<glm::vec2> &polyline)
{
  auto size = polyline.size();

  if (size > 1)
  {
    for (auto i = 0; i < size - 1; i++)
    {
      lineBatch.addVertex(polyline[i]);
      lineBatch.addVertex(polyline[i + 1]);
    }
  }
}

void Sketch::drawDot(float x, float y, float radius)
{
  Matrix matrix;
  matrix
    .translate(x, y)
    .scale(radius / DOT_RADIUS_PIXELS);

  draw::Sprite()
    .setAnchor(0.5f, 0.5f)
    .append(dotBatch, matrix);
}

void Sketch::initTextures()
{
  dotTexture = Texture(Texture::Request("dot_112.png")
    .setFlags(image::FLAGS_TRANSLUCENT)
    .setMipmap(true));
}
