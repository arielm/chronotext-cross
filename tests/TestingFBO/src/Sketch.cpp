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
  setupFramebuffer();

  texture = Texture(Texture::ImageRequest("lys_32.png")
    .setFlags(image::FLAGS_TRANSLUCENT_INVERSE)
    .setMipmap(true)
    .setWrap(GL_REPEAT, GL_REPEAT));

  //

  textureBatch
    .setShader(textureShader)
    .setShaderColor(1, 1, 1, 1)
    .setTexture(fboColorTexture);

  lightenBatch
    .setShader(lambertShader)
    .setShaderColor(0.25f, 0.25f, 0.25f, 1);

  // ---

  draw::Rect()
    .setBounds(-200, -200, 400, 400)
    .setTextureScale(400.0f / fboColorTexture.width)
    .append(textureBatch, Matrix());

  //

  draw::Cube()
    .setSize(150)
    .append(lightenBatch, Matrix());

  // ---

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glBindFramebuffer(GL_FRAMEBUFFER, fboId);
  drawScene2();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

//  fboColorTexture.bind();
//  glGenerateMipmap(GL_TEXTURE_2D);
//  fboColorTexture.unbind();

  drawScene1();
}

void Sketch::setupFramebuffer()
{
  fboColorTexture = Texture(
    Texture::EmptyRequest(512, 512));

  fboDepthTexture = Texture(
    Texture::EmptyRequest(512, 512)
      .setFormat(GL_DEPTH_COMPONENT)
      .setType(GL_FLOAT));

  glGenFramebuffersEXT(1, &fboId);
  glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, fboId);

  glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboColorTexture.textureId, 0);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fboDepthTexture.textureId, 0);

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
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
    .scale(1, -1, 1)
    .translate(0, 0, -400)
    .rotateY(clock()->getTime() * 0.0f); // XXX

  auto mvpMatrix = mvMatrix * projectionMatrix;

  // ---

  State()
    .setShaderMatrix<MVP>(mvpMatrix)
    .apply();

  textureBatch.flush();
}

void Sketch::drawScene2()
{
  glViewport(0, 0, fboColorTexture.width, fboColorTexture.height);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  glClearColor(0.5f, 0.5f, 0.5f, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ---

  auto projectionMatrix = glm::perspective(60 * D2R, fboColorTexture.width / fboColorTexture.height, 0.1f, 1000.0f);

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

  lightenBatch.flush();
}
