#include "Sketch.h"

#include "gl/draw/Texture.h"
#include "gl/draw/Rect.h"
#include "gl/draw/Circle.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace math;

void Sketch::setup()
{
  initTextures();

  // ---

  textureBatch.setShader(textureAlphaShader);
  textureBatch.setTexture(texture);

  colorBatch.setShader(colorShader);

  // ---

  glm::mat4 projectionMatrix = glm::ortho(0.0f, windowInfo.size.x, 0.0f, windowInfo.size.y);

  Matrix modelViewMatrix;

  modelViewMatrix.translate(windowInfo.size.x / 2, windowInfo.size.y / 2)
    .scale(1, -1);

  glm::mat4 mvpMatrix = modelViewMatrix * projectionMatrix;

  colorBatch.setShaderMatrix(mvpMatrix);
  textureBatch.setShaderMatrix(mvpMatrix);

  // ---

  Matrix matrix;

  matrix.push()
    .translate(200, 100)
    .rotateZ(30 * D2R);
  draw::Rect()
    .setColor(1, 1, 0.5f, 1)
    .fill(colorBatch, matrix, Rectf(-200, -150, 300, 150));
  matrix.pop();

  draw::Circle<XYZ.RGBA>(colorBatch)
    .setRadius(100)
    .fill(matrix, 0, 0, glm::vec4(1, 0.5f, 0, 1));

  matrix.push()
    .scale(0.5f)
    .rotateZ(-15 * D2R);
  draw::Texture<XYZ.UV.RGBA>(textureBatch).fillFromCenter(matrix, 0, 0, glm::vec4(1, 1, 1, 1));
  matrix.pop();

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

  colorBatch.flush();
  textureBatch.flush();
}

void Sketch::initTextures()
{
  texture = Texture(Texture::Request("lys_32.png")
                      .setFlags(image::FLAGS_TRANSLUCENT_INVERSE)
                      .setMipmap(true)
                      .setWrap(GL_REPEAT, GL_REPEAT));

  // ---

  #if defined(CHR_PLATFORM_EMSCRIPTEN)
    emscripten_webgl_enable_extension(emscripten_webgl_get_current_context(), "EXT_texture_filter_anisotropic");
    emscripten_webgl_enable_extension(emscripten_webgl_get_current_context(), "WEBKIT_EXT_texture_filter_anisotropic");
  #endif

  GLfloat maxAnisotropy;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
}
