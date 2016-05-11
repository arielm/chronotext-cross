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

  Triangulator triangulator;
  triangulator
    .setColor(1, 0.25f, 0.25f, 1)
    .add(starShape(100))
    .stamp(fillBatch, matrix);

  shapeToBatch(starShape(100), strokeBatch, matrix);

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

void Sketch::shapeToBatch(const vector<glm::vec2> &shape, IndexedVertexBatch<XYZ> &batch, Matrix &matrix)
{
  for (const auto &point : shape)
  {
    batch.addVertex(matrix.transformPoint(point));
  }

  int size = shape.size();

  for (int i = 0; i < size; i++)
  {
    batch.addIndices(i, (i + 1) % size);
  }

  batch.incrementIndices(size * 2);
}

vector<glm::vec2> Sketch::starShape(float outerRadius)
{
  constexpr float innerRadiusRatio =  0.38196601125f; // (3 - sqrt(5)) / 2
  float innerRadius = outerRadius * innerRadiusRatio;

  vector<glm::vec2> points;
  points.reserve(10);

  for (int i = 0; i < 5; i++)
  {
    /*
     * DRAWING IN CCW ORDER
     */

    float outerAngle = -i * 360 / 5.0f;
    float innerAngle = outerAngle - 360 / 10.0f;

    points.emplace_back(+sinf(outerAngle * D2R) * outerRadius, -cosf(outerAngle * D2R) * outerRadius);
    points.emplace_back(+sinf(innerAngle * D2R) * innerRadius, -cosf(innerAngle * D2R) * innerRadius);
  }

  return points;
}
