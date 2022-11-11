#include "Sketch.h"

#include "chr/gl/draw/Torus.h"
#include "chr/gl/draw/Sphere.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace draw;

static constexpr float RADIUS = 150;

Sketch::Sketch()
:
shader(InputSource::resource("Shader.vert"), InputSource::resource("Shader.frag"))
{}

void Sketch::setup()
{
  Torus()
    .setFrontFace(CW)
    .setSliceCount(20)
    .setLoopCount(60)
    .setInnerRadius(25)
    .setOuterRadius(100)
    .append(modelBatch, Matrix());

  modelBatch.setShader(shader);

  lightBatch
    .setShader(colorShader)
    .setShaderColor(1, 1, 1, 1);

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
    .translate(0, 0, -300);

  //

  float t = clock()->getTime() * 1.0f;
  float x = cosf(t) * RADIUS;
  float y = 0;
  float z = sinf(t) * RADIUS;
  glm::vec3 lightPosition(x, y, z);

  //

  State()
    .setShaderMatrix<MVP>(camera.getViewProjectionMatrix())
    .apply();

  lightBatch.clear();
  Sphere()
    .setFrontFace(CW)
    .setSectorCount(16)
    .setStackCount(8)
    .setRadius(4)
    .append(lightBatch, Matrix().translate(lightPosition));
  lightBatch.flush();

  //

  Matrix modelMatrix;
  modelMatrix.rotateY(clock()->getTime() * 0.125f);

  State()
    .setShaderMatrix<MODEL>(modelMatrix)
    .setShaderMatrix<VIEW>(camera.getViewMatrix())
    .setShaderMatrix<PROJECTION>(camera.getProjectionMatrix())
    .setShaderMatrix<NORMAL>(modelMatrix.getNormalMatrix())
    .setShaderUniform("u_view_pos", camera.getEyePosition())
    .setShaderUniform("u_material.point_light_count", 1)
    .setShaderUniform("u_point_lights[0].position", lightPosition)
    .setShaderUniform("u_point_lights[0].color", glm::vec3(1, 1, 1))
    .setShaderUniform("u_point_lights[0].intensity", 1.0f)
    .setShaderUniform("u_material.ambient", glm::vec3(0.1f, 0.1f, 0.1f))
    .setShaderUniform("u_material.diffuse", glm::vec3(1.0f, 0.5f, 0.0f))
    .setShaderUniform("u_material.specular", glm::vec3(1, 1, 1))
    .setShaderUniform("u_material.shininess", 25.0f)
    .apply();

  modelBatch.flush();
}
