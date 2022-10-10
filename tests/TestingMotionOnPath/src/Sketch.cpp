#include "Sketch.h"

#include "chr/gl/draw/Sprite.h"
#include "chr/gl/draw/Rect.h"
#include "chr/path/SplinePath.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;
using namespace path;

void Sketch::setup()
{
  dotTexture = Texture(
    Texture::ImageRequest("dot_112.png")
      .setFlags(image::FLAGS_TRANSLUCENT)
      .setMipmap(true));

  lineBatch
    .setPrimitive(GL_LINES)
    .setShader(colorShader)
    .setShaderColor(0, 0, 0, 0.67f);

  dotBatch
    .setShader(textureAlphaShader)
    .setShaderColor(1, 0, 0, 0.85f)
    .setTexture(dotTexture);

  flatBatch.setShader(colorShader);

  // ---

  path1
    .setMode(FollowablePath2D::MODE_LOOP)
    .begin()
    .add(450, 300)
    .add(500, 300)
    .add(500, 350)
    .add(450, 350)
    .end(true);

  drawPolyline(path1.getPoints());

  for (const auto &point : path1.getPoints())
  {
    drawDot(point.position, 2);
  }

  // ---

  path2
    .setMode(FollowablePath2D::MODE_MODULO)
    .begin()
    .add(200, 300)
    .add(350, 325)
    .add(450, 150)
    .add(500, 175)
    .add(550, 150)
    .end();

  drawPolyline(path2.getPoints());

  for (const auto &point : path2.getPoints())
  {
    drawDot(point.position, 2);
  }

  // ---

  MatrixAffine matrix;
  matrix
    .translate(150, 150)
    .rotate(-45 * D2R);

  SplinePath<glm::vec2> peanut;
  peanut
    .setType(SplinePath<glm::vec2>::TYPE_BSPLINE)
    .setSamplingTolerance(16)
    .add(-100, -100)
    .add(   0,  -25)
    .add( 100, -100)
    .add( 200,    0)
    .add( 100,  100)
    .add(   0,   25)
    .add(-100,  100)
    .add(-200,    0)
    .close()
    .transformPoints(matrix);

  path3
    .setMode(FollowablePath2D::MODE_LOOP)
    .begin()
    .add(peanut.getPolyline())
    .end();

  drawPolyline(path3.getPoints());

  for (const auto &point : path3.getPoints())
  {
    drawDot(point.position, 2);
  }

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  auto projectionMatrix = glm::ortho(0.0f, windowInfo.width, 0.0f, windowInfo.height);

  Matrix viewMatrix;
  viewMatrix
    .translate(0, windowInfo.height)
    .scale(1, -1);

  State()
    .setShaderMatrix<MVP>(viewMatrix * projectionMatrix)
    .apply();

  lineBatch.flush();
  dotBatch.flush();

  // ---

  flatBatch.clear();

  Matrix matrix1, matrix2, matrix3;

  path1
    .offsetToValue(clock()->getTime() * 40, 10)
    .applyToMatrix(matrix1);

  path2
    .offsetToValue(clock()->getTime() * -50, 20)
    .applyToMatrix(matrix2);

  path3
    .offsetToValue(clock()->getTime() * -30, 15)
    .applyToMatrix(matrix3);

  draw::Rect()
    .setColor(0, 0.50f, 0.75f, 0.75f)
    .setBounds(-5, -5, 10, 10)
    .append(flatBatch, matrix1);

  draw::Rect()
    .setColor(0, 0.75f, 0.25f, 0.75f)
    .setBounds(-10, -10, 20, 20)
    .append(flatBatch, matrix2);

  draw::Rect()
    .setColor(1, 0.5f, 0.25f, 0.75f)
    .setBounds(-7.5f, -7.5f, 15, 15)
    .append(flatBatch, matrix3);

  flatBatch.flush();
}

void Sketch::drawPolyline(const vector<FollowablePath2D::Point> &points)
{
  auto size = points.size();
  if (size > 1)
  {
    for (auto i = 0; i < size - 1; i++)
    {
      lineBatch.addVertices(points[i].position, points[i + 1].position);
    }
  }
}

void Sketch::drawDot(const glm::vec2 &position, float radius)
{
  static constexpr float DOT_RADIUS_PIXELS = 56; // Specific to "dot_112.png"

  draw::Sprite()
    .setAnchor(0.5f, 0.5f)
    .append(dotBatch, Matrix()
      .translate(position)
      .scale(radius / DOT_RADIUS_PIXELS));
}
