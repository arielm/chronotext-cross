#include "Sketch.h"

#include "chr/gl/draw/Torus.h"
#include "chr/gl/draw/Sphere.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace draw;

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
    .translate(0, 0, -300)
    .rotateY(clock()->getTime() * 0.125f);

  //

  float t = clock()->getTime() * 1.0f;
  float radius = 150;
  float x = cosf(t) * radius;
  float y = 0;
  float z = sinf(t) * radius;
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
    .append(lightBatch, Matrix().setTranslate(lightPosition));
  lightBatch.flush();

  //

  State()
    .setShader(shader)
    .setShaderMatrix<MODEL>(Matrix())
    .setShaderMatrix<VIEW>(camera.getViewMatrix())
    .setShaderMatrix<PROJECTION>(camera.getProjectionMatrix())
    .setShaderMatrix<NORMAL>(camera.getNormalMatrix())
    .setShaderUniform("u_light_position", lightPosition)
    .setShaderUniform("u_light_color", glm::vec3(1.0, 1.0, 1.0))
    .setShaderUniform("u_light_intensity", 1.0f)
    .setShaderUniform("u_ambient_color", glm::vec3(0, 0, 0))
    .setShaderUniform("u_diffuse_color", glm::vec3(1.0f, 0.5f, 0.0f))
    .setShaderUniform("u_specular_color", glm::vec3(1, 1, 1))
    .setShaderUniform("u_shininess", 25.0f)
    .apply();

  modelBatch.flush();
}
