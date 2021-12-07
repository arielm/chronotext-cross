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
  texture = Texture::ImageRequest("checker.png")
    .setWrap(GL_REPEAT, GL_REPEAT)
    .setFilters(GL_NEAREST, GL_NEAREST);

  geometryBatch.setTexture(texture);

  torusBatch
    .setShaderColor(1.0f, 0.5f, 0.0f, 1)
    .setTexture(texture);

  Box()
    .setFrontFace(CCW)
    .setColor(0.75f, 0.75f, 0.75f, 1)
    .setSize(300, 5, 300)
    .append(geometryBatch, Matrix());

  Box()
    .setFrontFace(CCW)
    .setColor(1, 0.25f, 0, 1)
    .setSize(60)
    .append(geometryBatch, Matrix().translate(0, -30, 0));

  Sphere()
    .setFrontFace(CCW)
    .setColor(0.25f, 1.0f, 0.0f, 1)
    .setSectorCount(60)
    .setStackCount(30)
    .setRadius(40)
    .append(geometryBatch, Matrix().translate(-75, -40, 100));

  Cylinder()
    .setFrontFace(CCW)
    .setColor(0.25f, 0.25f, 1.0f, 1)
    .setSectorCount(30)
    .setStackCount(1)
    .setBaseRadius(40)
    .setTopRadius(0)
    .setHeight(80)
    .append(geometryBatch, Matrix().translate(75, -40, -100).rotateX(D2R * 90));

  Cylinder()
    .setFrontFace(CCW)
    .setColor(0.25f, 0.25f, 1.0f, 1)
    .setSectorCount(30)
    .setStackCount(1)
    .setBaseRadius(40)
    .setTopRadius(40)
    .setHeight(80)
    .append(geometryBatch, Matrix().translate(-75, -40, -100).rotateX(D2R * 90));

  Torus()
    .setFrontFace(CCW)
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
    .scale(1, -1, 1)
    .translate(0, 0, -400)
    .rotateX(-30 * D2R)
    .rotateY(15 * D2R);

  State state;
  state
    .setShader(shader)
    .setShaderMatrix<MODEL>(Matrix())
    .setShaderMatrix<VIEW>(camera.getViewMatrix())
    .setShaderMatrix<PROJECTION>(camera.getProjectionMatrix())
    .setShaderMatrix<NORMAL>(camera.getNormalMatrix())
    .setShaderUniform("u_light_position", camera.getEyePosition())
    .setShaderUniform("u_light_color", glm::vec3(1.0, 1.0, 1.0))
    .setShaderUniform("u_light_intensity", 1.0f)
    .setShaderUniform("u_ambient_color", glm::vec3(0, 0, 0))
    .setShaderUniform("u_specular_color", glm::vec3(1, 1, 1))
    .setShaderUniform("u_shininess", 25.0f)
    .setShaderUniform("u_has_texture", true)
    .setShaderUniform("u_has_color", true) // i.e. do not use diffuse color but vertex color instead
    .apply();

  geometryBatch.flush();

  Matrix modelMatrix;
  modelMatrix
    .translate(75, -60, 100)
    .rotateY(clock()->getTime());

  Matrix modelViewMatrix = modelMatrix * camera.getViewMatrix();

  state
    .setShaderMatrix<MODEL>(modelMatrix)
    .setShaderMatrix<NORMAL>(modelViewMatrix.getNormalMatrix())
    .apply();

  torusBatch.flush();
}
