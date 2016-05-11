#include "Sketch.h"

#include "gl/draw/Texture.h"
#include "gl/draw/Rect.h"
#include "gl/draw/Circle.h"
#include "gl/Triangulator.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;

Sketch::Sketch()
:
strokeBatch(GL_LINES)
{}

void Sketch::setup()
{
  initTextures();

  // ---

  glm::mat4 projectionMatrix = glm::ortho(0.0f, windowInfo.width, 0.0f, windowInfo.height);

  Matrix modelViewMatrix;
  modelViewMatrix
    .translate(windowInfo.size / 2.0f)
    .scale(1, -1);

  // ---

  state
    .setShaderMatrix(modelViewMatrix * projectionMatrix)
    .glLineWidth(2);

  textureBatch
    .setShader(textureAlphaShader)
    .setTexture(texture);

  fillBatch
    .setShader(colorShader);

  strokeBatch
    .setShader(colorShader)
    .setShaderColor(1, 1, 1, 1);

  // ---

  Matrix matrix;

  matrix.push()
    .translate(200, 100)
    .rotateZ(30 * D2R);
  draw::Rect()
    .setColor(1, 1, 0.5f, 1)
    .fill(fillBatch, matrix, Rectf(-200, -150, 300, 150));
  matrix.pop();

  draw::Circle()
    .setColor(1, 0.5f, 0, 1)
    .setRadius(100)
    .fill(fillBatch, matrix);

  matrix.push()
    .scale(0.5f)
    .rotateZ(-15 * D2R);
  draw::Texture()
    .setColor(1, 1, 1, 1)
    .fillFromCenter(textureBatch, matrix, 0, 0);
  matrix.pop();

  Triangulator triangulator1;
  triangulator1
    .setContourCapture(Triangulator::CAPTURE_FRONT)
    .setColor(1, 0.25f, 0.25f, 1)
    .add(fivePointedStarShape(100))
    .stamp(fillBatch, matrix);

  triangulator1.exportContours(strokeBatch, matrix);

  matrix
    .translate(-200, 50)
    .rotateZ(30 * D2R);

  Triangulator triangulator2;
  triangulator2
    .setContourCapture(Triangulator::CAPTURE_FRONT)
    .setColor(0.25f, 1, 0, 1)
    .add(equilateralTriangleShape(150))
    .add(equilateralTriangleShape(120))
    .stamp(fillBatch, matrix);

  triangulator2.exportContours(strokeBatch, matrix);

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  state.apply();
  fillBatch.flush();
  textureBatch.flush();
  strokeBatch.flush();
}

void Sketch::initTextures()
{
  texture = Texture(Texture::Request("lys_32.png")
    .setFlags(image::FLAGS_TRANSLUCENT_INVERSE)
    .setMipmap(true)
    .setWrap(GL_REPEAT, GL_REPEAT));
}

vector<glm::vec2> Sketch::fivePointedStarShape(float outerRadius, float innerRadiusRatio)
{
  /*
   * THE DEFAULT INNER-RADIUS-RATIO IS EQUAL TO (3 - sqrt(5)) / 2
   * AS WELL AS 1 / (phi ^ 2), WHERE phi IS THE GOLDEN RATIO
   */

  float innerRadius = outerRadius * innerRadiusRatio;

  vector<glm::vec2> points;
  points.reserve(10);

  for (int i = 0; i < 5; i++)
  {
    /*
     * DRAWING IN CCW ORDER
     */

    float outerAngle = -i * TWO_PI / 5.0f;
    float innerAngle = outerAngle - TWO_PI / 10.0f;

    points.emplace_back(+sinf(outerAngle) * outerRadius, -cosf(outerAngle) * outerRadius);
    points.emplace_back(+sinf(innerAngle) * innerRadius, -cosf(innerAngle) * innerRadius);
  }

  return points;
}

vector<glm::vec2> Sketch::equilateralTriangleShape(float a)
{
  float h = sqrtf(3) * a / 2; // ALTITUDE FROM ANY SIDE

  vector<glm::vec2> points;
  points.reserve(3);

  points.emplace_back(0, -h * 2 / 3);
  points.emplace_back(-a / 2, h / 3);
  points.emplace_back(+a / 2, h / 3);

  return points;
}
