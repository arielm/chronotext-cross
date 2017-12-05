#include "Sketch.h"

#include "chr/gl/draw/Cube.h"

using namespace std;
using namespace chr;
using namespace gl;

Sketch::Sketch()
{}

void Sketch::setup()
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

  GLenum e = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
  switch (e)
  {
    case GL_FRAMEBUFFER_UNDEFINED:
      printf("FBO Undefined\n");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT :
      printf("FBO Incomplete Attachment\n");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT :
      printf("FBO Missing Attachment\n");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER :
      printf("FBO Incomplete Draw Buffer\n");
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED :
      printf("FBO Unsupported\n");
      break;
    case GL_FRAMEBUFFER_COMPLETE:
      printf("FBO OK\n");
      break;
    default:
      printf("FBO Problem?\n");
  }

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  //

  initTextures();

  lightenBatch.setTexture(texture);

  // ---

  Matrix matrix;

  draw::Cube()
    .setSize(150)
    .append(lightenBatch, matrix);

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

  glDepthMask(GL_FALSE);
  glDisable(GL_POLYGON_OFFSET_FILL);

  lightenBatch
    .setShader(textureAlphaShader)
    .setShaderColor(1, 1, 1, 0.5f)
    .flush();
}

void Sketch::initTextures()
{
  texture = Texture(
    Texture::ImageRequest("lys_32.png")
      .setFlags(image::FLAGS_TRANSLUCENT_INVERSE)
      .setMipmap(true)
      .setWrap(GL_REPEAT, GL_REPEAT)
      .setAnisotropy(true));
}
