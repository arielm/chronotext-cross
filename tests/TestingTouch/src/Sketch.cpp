
#include "Sketch.h"

using namespace std;
using namespace chr;

static const char *vss = R"(
attribute vec2 a_position;
attribute vec2 a_coord;
attribute vec4 a_color;

uniform mat4 u_mvp_matrix;

varying vec2 v_coord;
varying vec4 v_color;

void main()
{
  v_coord = a_coord;
  v_color = a_color;
  gl_Position = u_mvp_matrix * vec4(a_position, 0, 1);
}
)";

static const char *pss = R"(
#ifdef GL_ES
  precision mediump float;
#endif

uniform sampler2D u_sampler;

varying vec2 v_coord;
varying vec4 v_color;

void main()
{
  float alpha = texture2D(u_sampler, v_coord).a;
  gl_FragColor = vec4(v_color.rgb, alpha * v_color.a);
}
)";

static constexpr float DOT_RADIUS_DP = 22;
static constexpr float DOT_RADIUS_PIXELS = 56; // SPECIFIC TO "dot_112.png"
static constexpr float TEXTURE_RADIUS_PIXELS = 64; // SPECIFIC TO "dot_112.png"

void Sketch::setup()
{
  scale = getDisplayInfo().density / DisplayInfo::REFERENCE_DENSITY;
  projectionMatrix = glm::ortho(0.0f, windowInfo.size.x, windowInfo.size.y, 0.0f);

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
  glDisableVertexAttribArray(positionLocation);
  glDisableVertexAttribArray(coordLocation);
  glDeleteBuffers(3, vboIds);
  glDeleteTextures(1, textureIds);
}

void Sketch::draw()
{
  glClearColor(0, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  glEnableVertexAttribArray(positionLocation);
  glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
  glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(coordLocation);
  glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
  glVertexAttribPointer(coordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

  GLfloat color[4] = {1.0f, 1.0f, 0.0f, 1.0f};
  glVertexAttrib4fv(colorLocation, color);

  glActiveTexture(GL_TEXTURE0);
  glUniform1i(samplerLocation, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[2]);

  // ---

  for (auto &position : dotPositions)
  {
    drawDot(position, DOT_RADIUS_DP);
  }

  dotPositions.clear();
}

void Sketch::initBuffers()
{
  const GLfloat vertices[] =
  {
    -TEXTURE_RADIUS_PIXELS, +TEXTURE_RADIUS_PIXELS, // A
    -TEXTURE_RADIUS_PIXELS, -TEXTURE_RADIUS_PIXELS, // B
    +TEXTURE_RADIUS_PIXELS, -TEXTURE_RADIUS_PIXELS, // C
    +TEXTURE_RADIUS_PIXELS, +TEXTURE_RADIUS_PIXELS, // D
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 2, vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 2, coords, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indices, GL_STATIC_DRAW);
}

void Sketch::initTextures()
{
  textureIds[0] = loadTexture("dot_112.png", true);
}

void Sketch::initShaders()
{
  shaderProgram = makeShaderProgram(vss, pss);
  glUseProgram(shaderProgram);

  positionLocation = glGetAttribLocation(shaderProgram, "a_position");
  coordLocation = glGetAttribLocation(shaderProgram, "a_coord");
  colorLocation = glGetAttribLocation(shaderProgram, "a_color");
  samplerLocation = glGetUniformLocation(shaderProgram, "u_sampler");
  matrixLocation = glGetUniformLocation(shaderProgram, "u_mvp_matrix");
}

void Sketch::addTouch(int index, float x, float y)
{
  dotPositions.emplace_back(x, y);
}

void Sketch::updateTouch(int index, float x, float y)
{
  dotPositions.emplace_back(x, y);
}

void Sketch::drawDot(const glm::vec2 &position, float radius)
{
  glm::mat4 modelViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0));
  modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3(scale * radius / DOT_RADIUS_PIXELS));

  glm::mat4 mvp = projectionMatrix * modelViewMatrix;
  glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &mvp[0][0]);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}
