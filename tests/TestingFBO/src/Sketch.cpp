#include "Sketch.h"

#include "chr/gl/draw/Cube.h"
#include "chr/gl/draw/Rect.h"

using namespace std;
using namespace chr;
using namespace gl;

Sketch::Sketch()
{}

void Sketch::setup()
{
  fbo = FBO(512, 512);

  texture = Texture(Texture::ImageRequest("lys_32.png")
    .setFlags(image::FLAGS_TRANSLUCENT_INVERSE)
    .setMipmap(true)
    .setAnisotropy(true)
    .setWrap(GL_REPEAT, GL_REPEAT));

  textureBatch
    .setShader(textureShader)
    .setTexture(fbo.colorTexture);

  lightenBatch.setTexture(texture);

  // ---

  draw::Rect()
    .setBounds(-200, -200, 400, 400)
    .setColor(1, 1, 1, 1)
    .setTextureScale(400.0f / fbo.colorTexture.width)
    .append(textureBatch, Matrix());

  //

  draw::Cube()
    .setSize(150)
    .append(lightenBatch, Matrix());

  // ---

  glEnable(GL_BLEND);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // https://forum.openframeworks.cc/t/weird-problem-rendering-semi-transparent-image-to-fbo/2215/12
}

void Sketch::draw()
{
  fbo.bind();
  drawScene2(fbo.colorTexture.size);
  fbo.unbind();

//  fbo.colorTexture.bind();
//  glGenerateMipmap(GL_TEXTURE_2D);
//  fbo.colorTexture.unbind();

  drawScene1();
}

void Sketch::drawScene1()
{
  glViewport(0, 0, windowInfo.width, windowInfo.height);

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glClearColor(1, 0.75f, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  auto projectionMatrix = glm::perspective(60 * D2R, windowInfo.width / windowInfo.height, 0.1f, 1000.0f);

  Matrix mvMatrix;
  mvMatrix
    .scale(1, 1, 1)
    .translate(0, 0, -400)
    .rotateY(clock()->getTime() * 0.25f);

  auto mvpMatrix = mvMatrix * projectionMatrix;

  // ---

  State()
    .setShaderMatrix<MVP>(mvpMatrix)
    .apply();

  textureBatch.flush();
}

void Sketch::drawScene2(const glm::ivec2 &size)
{
  glViewport(0, 0, size.x, size.y);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  glClearColor(0.5f, 0.5f, 0.5f, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ---

  auto projectionMatrix = glm::perspective(60 * D2R, size.x / (float)size.y, 0.1f, 1000.0f);

  Matrix mvMatrix;
  mvMatrix
    .scale(1, -1, 1)
    .translate(0, 0, -300)
    .rotateY(clock()->getTime())
    .rotateZ(clock()->getTime() * 0.25f);

  auto mvpMatrix = mvMatrix * projectionMatrix;

  // ---

  State()
    .setShaderMatrix<MVP>(mvpMatrix)
    .setShaderMatrix<NORMAL>(mvMatrix.getNormalMatrix())
    .apply();

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2, 1);

  lightenBatch
    .setShader(lambertShader)
    .setShaderColor(0.25f, 0.25f, 0.25f, 1)
    .flush();

  glDepthMask(GL_FALSE);
  glDisable(GL_POLYGON_OFFSET_FILL);

  lightenBatch
    .setShader(textureAlphaShader)
    .setShaderColor(1, 1, 1, 0.5f)
    .flush();
}
