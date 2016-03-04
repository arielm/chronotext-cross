#include "Sketch.h"

#include "gl/TextureShader.h"

using namespace std;
using namespace chr;
using namespace gl;

void Sketch::setup()
{
  initTextures();
  textureBuffer.setup();

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::shutdown()
{
  glUseProgram(0);
  textureShader.unload();

  glDisableVertexAttribArray(textureShader.positionLocation);
  glDisableVertexAttribArray(textureShader.coordLocation);

  glDeleteTextures(1, &texture.id);
  textureBuffer.shutdown();
}

void Sketch::draw()
{
  glClearColor(0, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  textureShader.use();
  {
    glm::mat4 projectionMatrix = glm::perspective(60 * D2R, windowInfo.size.x / windowInfo.size.y, 0.1f, 100.0f);

    glm::mat4 modelViewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)); // SCALE
    modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0, 0, -30)); // DISTANCE
    modelViewMatrix = glm::rotate(modelViewMatrix, -15 * D2R, glm::vec3(1, 0, 0)); // ELEVATION
    modelViewMatrix = glm::rotate(modelViewMatrix, (float) getElapsedSeconds(), glm::vec3(0, 1, 0)); // AZIMUTH

    glm::mat4 mvp = projectionMatrix * modelViewMatrix;
    glUniformMatrix4fv(textureShader.matrixLocation, 1, GL_FALSE, &mvp[0][0]);

    // ---

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(textureShader.samplerLocation, 0);

    GLfloat color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    glVertexAttrib4fv(textureShader.colorLocation, color);

    textureBuffer.draw(texture, -400 * 0.03f, -300 * 0.03f, 0.03f); // XXX
  }
}

void Sketch::initTextures()
{
  image::ImageBuffer image = image::loadJpgImage("6980491_UN1_800.jpg", image::FLAGS_RBGA | image::FLAGS_POT);
  image::ImageBuffer mask = image::loadPngImage("6980491_UN1_800_MASK.png", image::FLAGS_TRANSLUCENT);
  texture = uploadMaskedTexture(image, mask);

  //texture = loadTexture("6980491_UN1_800.jpg", image::FLAGS_POT);
  //texture = loadTexture("banski.jpg", image::FLAGS_RBGA);
  //texture = loadTexture("camo.jpg", image::FLAGS_TRANSLUCENT);

  #if defined(CHR_PLATFORM_EMSCRIPTEN)
    emscripten_webgl_enable_extension(emscripten_webgl_get_current_context(), "EXT_texture_filter_anisotropic");
    emscripten_webgl_enable_extension(emscripten_webgl_get_current_context(), "WEBKIT_EXT_texture_filter_anisotropic");
  #endif

  GLfloat maxAnisotropy;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
  LOGI << "max-anisotropy: " << maxAnisotropy << endl;

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
}
