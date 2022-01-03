#include "Sketch.h"

#include "chr/gl/draw/Sphere.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace draw;
using namespace path;

constexpr float R1 = 6;
constexpr float R2 = 300;
constexpr float TURNS = 20;
constexpr float DD1 = 1.0f;
constexpr float DD2 = 3.0f;

constexpr int NUM_SPHERES = 1000;
constexpr float SWELL_FACTOR = 0.125f;

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
        .setSectorCount(20)
        .setStackCount(10)
        .setRadius(5)
        .append(batch, Matrix());

    instanceBuffer = InstanceBuffer(GL_DYNAMIC_DRAW);

    spiral.setup(surface, R1, R2, TURNS, DD1, DD2, 10 * NUM_SPHERES, 10);

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
        .setShaderMatrix<VIEW>(camera.getViewMatrix())
        .setShaderMatrix<PROJECTION>(camera.getProjectionMatrix())
        .setShaderUniform("u_light_position", camera.getEyePosition())
        .setShaderUniform("u_light_color", glm::vec3(1.0, 1.0, 1.0))
        .setShaderUniform("u_light_intensity", 1.0f)
        .setShaderUniform("u_ambient_color", glm::vec3(0, 0, 0))
        .setShaderUniform("u_specular_color", glm::vec3(1, 1, 1))
        .setShaderUniform("u_shininess", 25.0f)
        .setShaderUniform("u_has_texture", true)
        .setShaderUniform("u_has_color", true) // i.e. do not use diffuse color but vertex color instead
        .apply();

    threadSpiral(instanceBuffer, spiral.path, 10);
    batch.flush(instanceBuffer);

    State()
        .setShaderMatrix<MVP>(camera.getViewProjectionMatrix())
        .apply();
}

void Sketch::threadSpiral(InstanceBuffer &instanceBuffer, const FollowablePath3D &path, float spacing)
{
    instanceBuffer.clearMatrices();

    float offset = 0;
    Matrix matrix;

    for (int i = 0; i < NUM_SPHERES; i++)
    {
        auto value = path.offsetToValue(offset);
        value.applyToMatrix(matrix);
        instanceBuffer.addMatrix(matrix);
        offset += spacing;
    }
}
