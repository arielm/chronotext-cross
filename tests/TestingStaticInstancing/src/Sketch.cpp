#include "Sketch.h"
#include "BatchImporter.h"

#include "chr/gl/draw/Sphere.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace draw;

static constexpr float R1 = 250;
static constexpr float R2 = 150;
static constexpr float TURNS = 5;
static constexpr float H = 350;

Sketch::Sketch()
:
shader(InputSource::resource("Shader.vert"), InputSource::resource("Shader.frag"))
{}

void Sketch::setup()
{
  batch = BatchImporter::read(InputSource::resource("duck.model"))[0];

  instanceBuffer = InstanceBuffer(GL_STATIC_DRAW);

  threadHelix(instanceBuffer, R1, R2, TURNS, H, 0, 50);

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
  glClearColor(0.5f, 0.5f, 0.5f, 1);
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
    .setShaderUniform("u_light_position", camera.getEyePosition())
    .setShaderUniform("u_light_color", glm::vec3(1.0, 1.0, 1.0))
    .setShaderUniform("u_light_intensity", 1.0f)
    .apply();

  batch.flush(instanceBuffer);
}

void Sketch::threadHelix(InstanceBuffer &instanceBuffer, float r1, float r2, float turns, float h, float D, float spacing)
{
  float l = TWO_PI * turns;
  float L = PI * turns * (r1 + r2);
  float dz = h / l;
  float ay = -atan2f(h, L);

  float r;
  float dr;
  bool conical = (fabsf(r1 - r2) > 0.00001f); // AVOIDS INFINITY AND DIVISIONS-BY-ZERO WITH CYLINDRICAL HELICES (r1 = r2)

  if (conical)
  {
    dr = (r2 - r1) / l;
  }
  else
  {
    r = r1;
  }

  float d;
  float half = spacing * 0.5f;
  Matrix matrix;

  do
  {
    D += half;

    if (conical)
    {
      r = sqrtf(r1 * r1 + 2 * dr * D);
      d = (r - r1) / dr;
    }
    else
    {
      d = D / r;
    }

    matrix
      .setTranslate(-cosf(-d) * r, d * dz, +sinf(-d) * r)
      .rotateY(HALF_PI - d + 180 * D2R)
      .rotateZ(ay)
      .scale(0.5f);

    instanceBuffer.addMatrix(matrix);

    D += half;
  }
  while (D < L);
}
