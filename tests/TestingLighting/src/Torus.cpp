#include "Torus.h"

using namespace chr;
using namespace gl;

Torus& Torus::setFrontFace(GLenum mode)
{
    frontFace = mode;
    return *this;
}

Torus& Torus::setSliceCount(int count)
{
    sliceCount = count;
    return *this;
}

Torus& Torus::setLoopCount(int count)
{
    loopCount = count;
    return *this;
}

Torus& Torus::setInnerRadius(float radius)
{
    innerRadius = radius;
    return *this;
}

Torus& Torus::setOuterRadius(float radius)
{
    outerRadius = radius;
    return *this;
}

void Torus::append(IndexedVertexBatch<XYZ.N.UV> &batch, const Matrix &matrix) const
{
    for (int i = 0; i <= sliceCount; i++)
    {
        float v = i / float(sliceCount);
        float rho = v * TWO_PI;
        float cosRHO = cosf(rho);
        float sinRHO = sinf(rho);

        for (int j = 0; j <= loopCount; j++)
        {
            float u = j / float(loopCount);
            float theta = u * TWO_PI;
            float cosTHETA = cosf(theta);
            float sinTHETA = sinf(theta);

            float x = outerRadius * cosTHETA + innerRadius * cosTHETA * cosRHO;
            float y = outerRadius * sinTHETA + innerRadius * sinTHETA * cosRHO;
            float z = innerRadius * sinRHO;

            float nx = cosTHETA * cosRHO;
            float ny = sinTHETA * cosRHO;
            float nz = sinRHO;

            batch.addVertex(matrix.transformPoint(x, y, z), matrix.transformNormal(nx, ny, nz), glm::vec2(u, v));
        }
    }

    for (int i = 0; i < sliceCount; ++i)
    {
        int v1 = i * (loopCount + 1);
        int v2 = v1 + (loopCount + 1);

        for (int j = 0; j < loopCount; ++j)
        {
            if (frontFace == CCW)
            {
                batch.addIndices(v2, v2 + 1, v1, v1, v2 + 1, v1 + 1);
            }
            else
            {
                batch.addIndices(v2, v1, v2 + 1, v2 + 1, v1, v1 + 1);
            }

            v1 += 1;
            v2 += 1;
        }
    }
}
