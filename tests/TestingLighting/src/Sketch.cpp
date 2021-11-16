#include "Sketch.h"
#include "Torus.h"

using namespace std;
using namespace chr;
using namespace gl;

Sketch::Sketch()
:
phongShader(InputSource::resource("PhongShader.vert"), InputSource::resource("PhongShader.frag"))
{}

void Sketch::setup()
{
    fillBatch.setShaderColor(1.0f, 0.5f, 0.0f, 1);

    Torus()
            .setFrontFace(CW)
            .setSliceCount(20)
            .setLoopCount(60)
            .setInnerRadius(25)
            .setOuterRadius(100)
            .append(fillBatch, Matrix());

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
            .translate(0, 0, -300)
            .rotateY(clock()->getTime() * 0.5f);

    // ---

    State()
            .setShader(phongShader)
            .setShaderMatrix<MV>(camera.getMVMatrix())
            .setShaderMatrix<PROJECTION>(camera.getProjectionMatrix())
            .setShaderMatrix<NORMAL>(camera.getNormalMatrix())
            .apply();

    fillBatch.flush();
}
