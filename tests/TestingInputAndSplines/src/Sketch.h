#pragma once

#include "chr/cross/Context.h"
#include "chr/gl/Batch.h"
#include "chr/gl/shaders/ColorShader.h"
#include "chr/gl/shaders/TextureAlphaShader.h"

class Sketch : public chr::CrossSketch
{
public:
    void setup() final;
    void draw() final;

    void mouseDragged(int button, float x, float y) final;
    void mousePressed(int button, float x, float y) final;

    void keyDown(int keyCode, int modifiers) final;

protected:
    chr::gl::Texture dotTexture;

    chr::gl::IndexedVertexBatch<chr::gl::XYZ.UV.RGBA> dotBatch;
    chr::gl::VertexBatch<chr::gl::XYZ> lineBatch;

    chr::gl::shaders::ColorShader colorShader;
    chr::gl::shaders::TextureAlphaShader textureAlphaShader;

    std::vector<glm::vec2> points;
    int selectedIndex = -1;

    void updateSpline();
    void updateDots();
    int getClosestPointIndex(float x, float y);

    void drawPolyline(const std::vector<glm::vec2> &polyline);
    void drawDot(const glm::vec2 &position, const glm::vec4 &color, float radius);
};
