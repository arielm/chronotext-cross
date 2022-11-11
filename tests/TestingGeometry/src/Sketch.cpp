#include "Sketch.h"

#include "chr/gl/draw/Torus.h"
#include "chr/gl/draw/Sphere.h"
#include "chr/gl/draw/Cylinder.h"
#include "chr/gl/draw/Box.h"

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
  Texture texture = Texture::ImageRequest("checker.png")
    .setWrap(GL_REPEAT, GL_REPEAT)
    .setFilters(GL_NEAREST, GL_NEAREST);

  geometryBatch.setTexture(texture);

  torusBatch
    .setShaderColor(1.0f, 0.5f, 0.0f, 1)
    .setTexture(texture);

  Box()
    .setFrontFace(CW)
    .setColor(0.75f, 0.75f, 0.75f, 1)
    .setSize(300, 0, 300)
    .append(geometryBatch, Matrix());

  Box()
    .setFrontFace(CW)
    .setColor(1, 0.25f, 0, 1)
    .setSize(60)
    .append(geometryBatch, Matrix().translate(0, 30, 0));

  Sphere()
    .setFrontFace(CW)
    .setColor(0.25f, 1.0f, 0.0f, 1)
    .setSectorCount(60)
    .setStackCount(30)
    .setRadius(40)
    .append(geometryBatch, Matrix().translate(-75, 40, 100));

  Cylinder()
    .setFrontFace(CW)
    .setColor(0.25f, 0.25f, 1.0f, 1)
    .setSectorCount(30)
    .setStackCount(1)
    .setBaseRadius(40)
    .setTopRadius(0)
    .setHeight(80)
    .append(geometryBatch, Matrix().translate(75, 40, -100).rotateX(-HALF_PI));

  Cylinder()
    .setFrontFace(CW)
    .setColor(0.25f, 0.25f, 1.0f, 1)
    .setSectorCount(30)
    .setStackCount(1)
    .setBaseRadius(40)
    .setTopRadius(40)
    .setHeight(80)
    .append(geometryBatch, Matrix().translate(-75, 40, -100).rotateX(-HALF_PI));

  Torus()
    .setFrontFace(CW)
    .setSliceCount(20)
    .setLoopCount(60)
    .setInnerRadius(12)
    .setOuterRadius(48)
    .append(torusBatch, Matrix());

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

void Sketch::draw()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ---

  camera.getViewMatrix()
    .setIdentity()
    .translate(0, 0, -400)
    .rotateX(30 * D2R)
    .rotateY(15 * D2R);

  Matrix modelMatrix;

  State state;
  state
    .setShader(shader)
    .setShaderMatrix<MODEL>(modelMatrix)
    .setShaderMatrix<VIEW>(camera.getViewMatrix())
    .setShaderMatrix<PROJECTION>(camera.getProjectionMatrix())
    .setShaderMatrix<NORMAL>(modelMatrix.getNormalMatrix())
    .setShaderUniform("u_view_pos", camera.getEyePosition())
    .setShaderUniform("u_material.point_light_count", 1)
    .setShaderUniform("u_point_lights[0].position", camera.getEyePosition())
    .setShaderUniform("u_point_lights[0].color", glm::vec3(1, 1, 1))
    .setShaderUniform("u_point_lights[0].intensity", 1.0f)
    .setShaderUniform("u_material.ambient", glm::vec3(0.1f, 0.1f, 0.1f))
    .setShaderUniform("u_material.specular", glm::vec3(1, 1, 1))
    .setShaderUniform("u_material.shininess", 25.0f)
    .setShaderUniform("u_material.has_color", true) // i.e. do not use diffuse color but vertex color instead
    .setShaderUniform("u_material.has_texture", true)
    .setShaderUniform("u_material.texture", 0)
    .apply();

  geometryBatch.flush();

  modelMatrix
    .translate(75, 60, 100)
    .rotateY(clock()->getTime());

  state
    .setShaderMatrix<MODEL>(modelMatrix)
    .setShaderMatrix<NORMAL>(modelMatrix.getNormalMatrix())
    .apply();

  torusBatch.flush();
}
