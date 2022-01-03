#include "Sketch.h"
#include "BatchImporter.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace path;

constexpr float R1 = 6;
constexpr float R2 = 300;
constexpr float TURNS = 20;
constexpr float DD1 = 1.0f;
constexpr float DD2 = 3.0f;

constexpr int NUM_DUCKS = 1000;
constexpr float SWELL_FACTOR = 0.125f;

Sketch::Sketch()
:
shader(InputSource::resource("Shader.vert"), InputSource::resource("Shader.frag"))
{}

void Sketch::setup()
{
  batch = BatchImporter::read(InputSource::resource("duck.model"))[0];

  instanceBuffer = InstanceBuffer(GL_DYNAMIC_DRAW);

  spiral.setup(surface, R1, R2, TURNS, DD1, DD2, 10 * NUM_DUCKS, 10);

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
    .setFov(45)
    .setClip(0.1f, 1000.0f)
    .setWindowSize(windowInfo.size);
}

void Sketch::update()
{
  spiral.update(surface, clock()->getTime(), SWELL_FACTOR);
}

void Sketch::draw()
{
  glClearColor(0.4f, 0.8f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ---

  camera.getViewMatrix()
    .setIdentity()
    .translate(0, 0, -300)
    .rotateX(115 * D2R)
    .rotateY(0);

  State()
    .setShader(shader)
    .setShaderMatrix<VIEW>(camera.getViewMatrix())
    .setShaderMatrix<PROJECTION>(camera.getProjectionMatrix())
    .setShaderUniform("u_light_position", camera.getEyePosition())
    .setShaderUniform("u_light_color", glm::vec3(1.0, 1.0, 1.0))
    .setShaderUniform("u_light_intensity", 1.0f)
    .apply();

  threadSpiral(instanceBuffer, spiral.path, 10);
  batch.flush(instanceBuffer);
}

void Sketch::threadSpiral(InstanceBuffer &instanceBuffer, const FollowablePath3D &path, float spacing)
{
  instanceBuffer.clearMatrices();

  float offset = 0;
  Matrix matrix;

  for (int i = 0; i < NUM_DUCKS; i++)
  {
    auto value = path.offsetToValue(offset);
    value.applyToMatrix(matrix);
    matrix
      .rotateX(-90 * D2R)
      .scale(0.1f);
    instanceBuffer.addMatrix(matrix);
    offset += spacing;
  }
}
