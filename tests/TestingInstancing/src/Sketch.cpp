#include "Sketch.h"

#include "chr/gl/draw/Sphere.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace draw;

static constexpr float R1 = 250;
static constexpr float R2 = 150;
static constexpr float TURNS = 6;
static constexpr float H = 350;

Sketch::Sketch()
:
shader(InputSource::resource("Shader.vert"), InputSource::resource("Shader.frag"))
{}

void Sketch::setup()
{
  texture = Texture::ImageRequest("checker.png")
    .setFlags(image::FLAGS_RBGA)
    .setFilters(GL_NEAREST, GL_NEAREST);

  batch
    .setShader(shader)
    .setShaderColor(0.25f, 1.0f, 0.0f, 1)
    .setTexture(texture);

  Sphere()
    .setFrontFace(CW)
    .setSectorCount(40)
    .setStackCount(20)
    .setRadius(25)
    .append(batch, Matrix());

  instanceBuffer = InstanceBuffer(GL_STATIC_DRAW, GL_STATIC_DRAW);

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

  camera.getMVMatrix()
    .setIdentity()
    .translate(0, -150, -600)
    .rotateY(clock()->getTime() * 0.125f);


  State()
    .setShaderMatrix<MV>(camera.getMVMatrix())
    .setShaderMatrix<PROJECTION>(camera.getProjectionMatrix())
    .setShaderUniform("u_eye_position", camera.getEyePosition())
    .setShaderUniform("u_light_position", camera.getEyePosition())
    .setShaderUniform("u_shininess", 50.0f)
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
      .rotateY(HALF_PI - d)
      .rotateZ(ay);

    instanceBuffer.addMatrix(matrix);

    D += half;
  }
  while (D < L);
}
