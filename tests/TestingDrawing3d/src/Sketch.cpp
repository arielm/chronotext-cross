#include "Sketch.h"

#include "gl/draw/Box.h"
#include "gl/draw/Cube.h"

using namespace std;
using namespace chr;
using namespace gl;

Sketch::Sketch()
{}

void Sketch::setup()
{
  initTextures();

  lightenBatch.setTexture(texture);
  flatBatch.setTexture(texture);

  // ---

  Matrix matrix;

  draw::Box()
    .setSize(100, 75, 25)
    .setTextureScale(0.25f)
    .append(lightenBatch, matrix);

  matrix
    .push()
    .translate(50, 0, 0);

  draw::Cube()
    .setSize(50)
    .append(flatBatch, matrix);

  matrix
    .pop()
    .translate(-50, 0, 0);

  draw::Cube()
    .setSize(50)
    .append(flatBatch, matrix);

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

  state
    .setShaderMatrix<MVP>(mvpMatrix)
    .setShaderMatrix<NORMAL>(mvMatrix.getNormalMatrix())
    .apply();

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2, 1);

  lightenBatch
    .setShader(lambertShader)
    .setShaderColor(0.25f, 0.25f, 0.25f, 1)
    .flush();

  flatBatch
    .setShader(colorShader)
    .setShaderColor(1, 0.25f, 0, 1)
    .flush();

  glDepthMask(GL_FALSE);
  glDisable(GL_POLYGON_OFFSET_FILL);

  lightenBatch
    .setShader(textureAlphaShader)
    .setShaderColor(1, 1, 1, 0.5f)
    .flush();

  flatBatch
    .setShader(textureAlphaShader)
    .setShaderColor(1, 1, 1, 1)
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
