#include "Sketch.h"
#include "BatchImporter.h"

using namespace std;
using namespace chr;
using namespace gl;

static constexpr float R1 = 250;
static constexpr float R2 = 150;
static constexpr float TURNS = 7.5f;
static constexpr float H = 350;

Sketch::Sketch()
:
shader(InputSource::resource("Shader.vert"), InputSource::resource("Shader.frag"))
{}

void Sketch::setup()
{
  batch = BatchImporter().import(InputSource::resource("duck.model"))[0];

  instanceBuffer = InstanceBuffer(GL_STATIC_DRAW);

  drawOnHelix(instanceBuffer, R1, R2, TURNS, H, 33, 0.33f);

  // ---

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::resize()
{
  camera
    .setFov(60)
    .setClip(0.1f, 1000.0f)
    .setWindowSize(windowInfo.size);
}

void Sketch::draw()
{
  glClearColor(0.4f, 0.8f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ---

  camera.getViewMatrix()
    .setIdentity()
    .translate(0, -200, -600)
    .rotateY(clock()->getTime() * 0.125f);

  State()
    .setShader(shader)
    .setShaderMatrix<VIEW>(camera.getViewMatrix())
    .setShaderMatrix<PROJECTION>(camera.getProjectionMatrix())
    .setShaderUniform("u_view_pos", camera.getEyePosition())
    .setShaderUniform("u_material.point_light_count", 1)
    .setShaderUniform("u_point_lights[0].position", camera.getEyePosition())
    .setShaderUniform("u_point_lights[0].color", glm::vec3(1, 1, 1))
    .setShaderUniform("u_point_lights[0].intensity", 1.0f)
    .setShaderUniform("u_material.texture", 0)
    .apply();

  batch.flush(instanceBuffer);
}

void Sketch::drawOnHelix(InstanceBuffer &instanceBuffer, float r1, float r2, float turns, float h, float spacing, float scale)
{
  float l = TWO_PI * turns;
  float L = PI * turns * (r1 + r2);
  float dz = h / l;
  float ay = -atan2f(h, L);

  float r;
  float dr;
  bool conical = (fabsf(r1 - r2) > 0.00001f); // Avoids infinity and divisions-by-zeo with cylindrical helices (r1 = r2)

  if (conical)
  {
    dr = (r2 - r1) / l;
  }
  else
  {
    r = r1;
  }

  float offset = 0;
  float half = spacing * 0.5f;
  Matrix matrix;

  do
  {
    offset += half;

    float d;
    if (conical)
    {
      r = sqrtf(r1 * r1 + 2 * dr * offset);
      d = (r - r1) / dr;
    }
    else
    {
      d = offset / r;
    }

    matrix
      .setTranslate(-cosf(-d) * r, d * dz, +sinf(-d) * r)
      .rotateY(HALF_PI - d + PI)
      .rotateZ(ay)
      .scale(scale);

    instanceBuffer.addMatrix(matrix);

    offset += half;
  }
  while (offset < L);
}
