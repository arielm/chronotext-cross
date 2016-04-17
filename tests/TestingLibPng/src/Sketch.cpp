#include "Sketch.h"

#include "gl/geom/TextureRect.h"

using namespace std;
using namespace chr;
using namespace gl;

void Sketch::setup()
{
  initTextures();

  // ---

  textureBatches[0].setShader(textureShader);
  textureBatches[0].setShaderColor(1, 1, 1, 1);
  textureBatches[0].setTexture(textures[0]);

  textureBatches[1].setShader(textureAlphaShader);
  textureBatches[1].setShaderColor(1, 0.5f, 0, 1);
  textureBatches[1].setTexture(textures[1]);

  textureBatches[2].setShader(textureShader);
  textureBatches[2].setShaderColor(1, 1, 1, 1);
  textureBatches[2].setTexture(textures[2]);

  Matrix matrix;

  matrix.push();
  matrix.scale(0.333f);
  geom::TextureRect<UV>(textureBatches[0]).drawFromCenter(matrix);
  matrix.pop();

  matrix.translate(0, 0, 5);
  geom::TextureRect<UV>(textureBatches[1]).drawInRect(matrix, math::Rectf(-200, -150, 400, 300));

  matrix.translate(0, 0, 5);
  matrix.scale(0.75f);
  geom::TextureRect<UV>(textureBatches[2]).drawFromCenter(matrix, 100, 100);

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(0, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  glm::mat4 projectionMatrix = glm::perspective(60 * D2R, windowInfo.size.x / windowInfo.size.y, 0.1f, 1000.0f);

  glm::mat4 modelViewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1, -1, 1)); // SCALE
  modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0, 0, -300)); // DISTANCE
  modelViewMatrix = glm::rotate(modelViewMatrix, 15 * D2R, glm::vec3(1, 0, 0)); // ELEVATION
  modelViewMatrix = glm::rotate(modelViewMatrix, (float) getElapsedSeconds(), glm::vec3(0, 1, 0)); // AZIMUTH

  glm::mat4 mvpMatrix = projectionMatrix * modelViewMatrix;

  // ---

  textureBatches[0].setShaderMatrix(mvpMatrix);
  textureBatches[1].setShaderMatrix(mvpMatrix);
  textureBatches[2].setShaderMatrix(mvpMatrix);

  textureBatches[0].flush();
  textureBatches[1].flush();
  textureBatches[2].flush();
}

void Sketch::initTextures()
{
  textures[0] = Texture(Texture::Request("6980491_UN1_800_MASK.png").setFlags(image::FLAGS_POT));
  textures[1] = Texture(Texture::Request("lys_32.png").setFlags(image::FLAGS_TRANSLUCENT_INVERSE).setMipmap(true).setWrap(GL_REPEAT, GL_REPEAT));
  textures[2] = Texture(Texture::Request("expo67.png"));

  // ---

  #if defined(CHR_PLATFORM_EMSCRIPTEN)
    emscripten_webgl_enable_extension(emscripten_webgl_get_current_context(), "EXT_texture_filter_anisotropic");
    emscripten_webgl_enable_extension(emscripten_webgl_get_current_context(), "WEBKIT_EXT_texture_filter_anisotropic");
  #endif

  GLfloat maxAnisotropy;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
}
