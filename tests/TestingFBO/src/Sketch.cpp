#include "Sketch.h"

#include "chr/gl/draw/Cube.h"

using namespace std;
using namespace chr;
using namespace gl;

GLuint createRGBATexture(int w, int h) {

  GLuint tex;

  glGenTextures(1, &tex);

  glBindTexture(GL_TEXTURE_2D, tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
               w,
               h,
               0, GL_RGBA, GL_UNSIGNED_BYTE,
               NULL);

  glBindTexture(GL_TEXTURE_2D, 0);

  return(tex);
}

GLuint createDepthTexture(int w, int h) {

  GLuint tex;

  glGenTextures(1, &tex);

  glBindTexture(GL_TEXTURE_2D, tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
               w,
               h,
               0, GL_DEPTH_COMPONENT, GL_FLOAT,
               NULL);

  glBindTexture(GL_TEXTURE_2D, 0);

  return(tex);
}

GLuint prepareFBO(int w, int h) {

  // Generate one frame buffer
  GLuint fbo;
  glGenFramebuffersEXT(1, &fbo);

  // bind it
  glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, fbo);

  // attach textures for colors
  GLuint texFBO = createRGBATexture(w,h);
  glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texFBO, 0);

  GLuint depthFBO;
  depthFBO = createDepthTexture(w,h);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthFBO, 0);

  // check if everything is OK
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

  if (e != GL_FRAMEBUFFER_COMPLETE)
    return (0);

  // unbind fbo
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  return fbo;
}

Sketch::Sketch()
{}

void Sketch::setup()
{
  prepareFBO(512, 512);

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
