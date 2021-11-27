#include "Sketch.h"

#include "chr/math/MatrixAffine.h"
#include "chr/gl/Triangulator.h"
#include "chr/gl/draw/Sprite.h"
#include "chr/shape/FivePointedStar.h"
#include "chr/Random.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;
using namespace path;
using namespace draw;

Sketch::Sketch()
{}

void Sketch::setup()
{
  loadMoon();
  loadUFO();

  starsBatch
    .setShader(colorShader)
    .setShaderColor(1, 1, 1, 1);

  moonBatch
    .setShader(colorShader)
    .setShaderColor(1, 1, 0.75f, 1);

  gradientBatch.setShader(colorShader);

  ufoBatch
    .setShader(textureShader)
    .setShaderColor(1, 1, 1, 1)
    .setTexture(ufoTexture);

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::resize()
{
  createGradient();
  createStars(150);
  placeMoon();
}

void Sketch::draw()
{
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  glm::mat4 projectionMatrix = glm::ortho(0.0f, windowInfo.width, 0.0f, windowInfo.height);

  Matrix modelViewMatrix;
  modelViewMatrix
    .translate(0, windowInfo.height)
    .scale(1, -1);

  State()
    .setShaderMatrix(modelViewMatrix * projectionMatrix)
    .apply();

  gradientBatch.flush();
  starsBatch.flush();
  moonBatch.flush();

  drawUFO();
}

void Sketch::createGradient()
{
  float x1 = 0;
  float y1 = 0;
  float x2 = windowInfo.width;
  float y2 = windowInfo.height;

  glm::vec4 color1(0, 0, 0, 0);
  glm::vec4 color2(1, 0, 1, 0.33f);

  gradientBatch.clear();

  gradientBatch
    .addVertex(x1, y1, color1)
    .addVertex(x2, y1, color1)
    .addVertex(x2, y2, color2)
    .addVertex(x1, y2, color2);

  gradientBatch.addIndices(0, 1, 2, 2, 3, 0);
}

void Sketch::createStars(int n)
{
  float width = windowInfo.width;
  float height = windowInfo.height;

  MatrixAffine affine;
  Triangulator triangulator;
  Random rnd(42);

  auto starPoints = shape::FivePointedStar()
    .setOuterRadius(1)
    .setInnerRadiusRatio(0.333f)
    .append();

  starsBatch.clear();

  for (int i = 0; i < n; i++)
  {
    glm::vec2 position(rnd.nextFloat(width), rnd.nextFloat(height));
    float scale = rnd.nextFloat(3, 15);
    float rotation = rnd.nextFloat(0, 360);

    Shape star;
    star
      .addPath(affine
         .setTranslate(position)
         .scale(scale)
         .rotate(rotation * D2R)
         .transformPoints(starPoints));

    triangulator
      .add(star)
      .fill(starsBatch);
  }
}

void Sketch::loadMoon()
{
  moonDocument
    .setOriginAtBottom(false)
    .setSamplingTolerance(50)
    .load(InputSource::resource("moon.svg"));
}

void Sketch::placeMoon()
{
  Triangulator triangulator;

  for (auto &shape : moonDocument.getShapes())
  {
    triangulator.add(shape);
  }

  moonBatch.clear();

  triangulator.fill(moonBatch, Matrix()
    .translate(windowInfo.width * 0.25f, windowInfo.height * 0.125f)
    .scale(0.5f));
}

void Sketch::loadUFO()
{
  ufoTexture = Texture(Texture::ImageRequest("ufo.png")
    .setFlags(image::FLAGS_RBGA)
    .setMipmap(true));
}

void Sketch::drawUFO()
{
  ufoBatch.clear();

  Sprite()
    .setAnchor(0.5f, 0.5f)
    .append(ufoBatch, Matrix()
      .translate(windowInfo.center())
      .rotateZ(sinf(clock()->getTime() * 3.0f) * 0.125f)
      .scale(0.5f));

  ufoBatch.flush();
}
