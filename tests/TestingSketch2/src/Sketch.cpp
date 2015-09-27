
#include "Sketch.h"

using namespace std;
using namespace chr;

void Sketch::setup()
{
  initBuffers();
  initTextures();
  initShaders();

  // ---

  glEnable(GL_TEXTURE_2D);

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::shutdown()
{
  glUseProgram(0);
  shaderProgram.unload();

  glDisableVertexAttribArray(shaderProgram.positionLocation);
  glDisableVertexAttribArray(shaderProgram.coordLocation);

  glDeleteBuffers(3, vboIds);
  glDeleteTextures(1, textureIds);
}

void Sketch::draw()
{
  glm::mat4 projectionMatrix = glm::perspective(60 * D2R, windowInfo.size.x / windowInfo.size.y, 0.1f, 100.0f);

  glm::mat4 modelViewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)); // SCALE
  modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0, 0, -30)); // DISTANCE
  modelViewMatrix = glm::rotate(modelViewMatrix, -15 * D2R, glm::vec3(1, 0, 0)); // ELEVATION
  modelViewMatrix = glm::rotate(modelViewMatrix, (float)getElapsedSeconds(), glm::vec3(0, 1, 0)); // AZIMUTH

  glm::mat4 mvp = projectionMatrix * modelViewMatrix;
  glUniformMatrix4fv(shaderProgram.matrixLocation, 1, GL_FALSE, &mvp[0][0]);

  // ---

  glClearColor(0, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
  glEnableVertexAttribArray(shaderProgram.positionLocation);
  glVertexAttribPointer(shaderProgram.positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
  glEnableVertexAttribArray(shaderProgram.coordLocation);
  glVertexAttribPointer(shaderProgram.coordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

  GLfloat color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  glVertexAttrib4fv(shaderProgram.colorLocation, color);

  glActiveTexture(GL_TEXTURE0);
  glUniform1i(shaderProgram.samplerLocation, 0);

  // ---

  glUseProgram(shaderProgram.id);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[2]);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

void Sketch::initBuffers()
{
  const GLfloat vertices[] =
  {
    -10, +5, 0, // A
    -10, -5, 0, // B
    +10, -5, 0, // C
    +10, +5, 0, // D
  };

  const GLfloat coords[] =
  {
    0, 0, // A
    0, 1, // B
    1, 1, // C
    1, 0, // D
  };

  const GLushort indices[] =
  {
    0, 1, 2,
    2, 3, 0
  };

  glGenBuffers(3, vboIds);

  glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 3, vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 2, coords, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indices, GL_STATIC_DRAW);
}

void Sketch::initTextures()
{
  textureIds[0] = gl::loadTexture("expo67.png");

  /*
   * PROBLEMS:
   * - DESKTOP AND EMSCRIPTEN: GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT IS NOT RECOGNIZED
   * - EMSCRIPTEN: RETURNS 0
   */
  glGetIntegerv(0x84FF, &maxAnisotropy);
  LOGI << "max-anisotropy: " << maxAnisotropy << endl;

  glTexParameteri( GL_TEXTURE_2D, 0x84FF, maxAnisotropy);
}

void Sketch::initShaders()
{
  shaderProgram.load();
}
