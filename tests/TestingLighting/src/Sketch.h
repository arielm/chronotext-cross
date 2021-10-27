#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Camera.h"
#include "chr/gl/Batch.h"
#include "chr/gl/ShaderProgram.h"

class Sketch : public chr::CrossSketch
{
public:
    Sketch();
    virtual ~Sketch() {}

    void setup() final;
    void resize() final;
    void draw() final;

protected:
    chr::gl::Camera camera;

    chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV> fillBatch;
    chr::gl::ShaderProgram phongShader;

    static void generateTorus(chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV> &batch, int slices, int loops, float innerRadius, float outerRadius);
};
