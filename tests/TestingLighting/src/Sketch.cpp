#include "Sketch.h"

using namespace std;
using namespace chr;
using namespace gl;

Sketch::Sketch()
:
phongShader(InputSource::resource("PhongShader.vert"), InputSource::resource("PhongShader.frag"))
{}

void Sketch::setup()
{
    generateTorus(fillBatch, 20, 60, 25, 100);

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
            .scale(1, -1, 1)
            .translate(0, 0, -300)
            .rotateY(clock()->getTime() * 0.5f);

    // ---

    State()
            .setShader(phongShader)
            .setShaderColor(1.0f, 0.5f, 0.0f, 1)
            .setShaderMatrix<MV>(camera.getMVMatrix())
            .setShaderMatrix<PROJECTION>(camera.getProjectionMatrix())
            .setShaderMatrix<NORMAL>(camera.getNormalMatrix())
            .apply();

    fillBatch
            .flush();
}

void Sketch::generateTorus(IndexedVertexBatch<XYZ.N.UV> &batch, int slices, int loops, float innerRadius, float outerRadius)
{
    for (int i = 0; i <= slices; i++)
    {
        float v = i / float(slices);
        float rho = v * TWO_PI;
        float cosRHO = cosf(rho);
        float sinRHO = sinf(rho);

        for (int j = 0; j <= loops; j++)
        {
            float u = j / float(loops);
            float theta = u * TWO_PI;
            float cosTHETA = cosf(theta);
            float sinTHETA = sinf(theta);

            float x = outerRadius * cosTHETA + innerRadius * cosTHETA * cosRHO;
            float y = outerRadius * sinTHETA + innerRadius * sinTHETA * cosRHO;
            float z = innerRadius * sinRHO;

            float nx = cosTHETA * cosRHO;
            float ny = sinTHETA * cosRHO;
            float nz = sinRHO;

            batch.addVertex(glm::vec3(x, y, z), glm::vec3(nx, ny, nz), glm::vec2(u, v));
        }
    }

    for (int i = 0; i < slices; ++i)
    {
        int v1 = i * (loops + 1);
        int v2 = v1 + (loops + 1);

        for (int j = 0; j < loops; ++j)
        {
            batch.addIndices(v2, v2 + 1, v1, v1, v2 + 1, v1 + 1);
            v1 += 1;
            v2 += 1;
        }
    }
}
