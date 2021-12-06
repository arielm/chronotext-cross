#include "Sketch.h"

#include "chr/gl/draw/Box.h"
#include "chr/gl/draw/Rect.h"

using namespace std;
using namespace chr;
using namespace gl;

Sketch::Sketch()
{}

void Sketch::setup()
{
  fbo = FBO(512, 512);

  texture = Texture(Texture::ImageRequest("lys.png")
    .setMipmap(true)
    .setAnisotropy(true));

  textureBatch
    .setShader(textureShader)
    .setShaderColor(1, 1, 1, 1)
    .setTexture(fbo.colorTexture);

  cubeBatch
    .setTexture(texture)
    .setShader(textureLambertShader)
    .setShaderColor(0.25f, 0.25f, 0.25f, 1);

  draw::Rect()
    .setBounds(-200, -200, 400, 400)
    .setTextureScale(400.0f / fbo.colorTexture.width)
    .append(textureBatch, Matrix());

  draw::Box()
    .setSize(150)
    .append(cubeBatch, Matrix());

  // ---

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // https://www.opengl.org/discussion_boards/showthread.php/167554-FBO-and-blending
}

void Sketch::draw()
{
  fbo.bind();
  drawScene2(fbo.colorTexture.size);
  fbo.unbind();

  drawScene1();
}

void Sketch::drawScene1()
{
  glDisable(GL_CULL_FACE);

  glViewport(0, 0, windowInfo.width, windowInfo.height);

  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ---

  auto projectionMatrix = glm::perspective(60 * D2R, windowInfo.aspectRatio(), 0.1f, 1000.0f);

  Matrix viewMatrix;
  viewMatrix
    .translate(0, 0, -400)
    .rotateY(clock()->getTime() * 0.25f);

  State()
    .setShaderMatrix<MVP>(viewMatrix * projectionMatrix)
    .apply();

  textureBatch.flush();
}

void Sketch::drawScene2(const glm::ivec2 &size)
{
  glEnable(GL_CULL_FACE);

  glViewport(0, 0, size.x, size.y);

  glClearColor(0.5f, 0.5f, 0.5f, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ---

  auto projectionMatrix = glm::perspective(60 * D2R, size.x / (float)size.y, 0.1f, 1000.0f);

  Matrix viewMatrix;
  viewMatrix
    .scale(1, -1, 1)
    .translate(0, 0, -300)
    .rotateY(clock()->getTime())
    .rotateZ(clock()->getTime() * 0.25f);

  State()
    .setShaderMatrix<MVP>(viewMatrix * projectionMatrix)
    .setShaderMatrix<NORMAL>(viewMatrix.getNormalMatrix())
    .apply();

  cubeBatch.flush();
}
