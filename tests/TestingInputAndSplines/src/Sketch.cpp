#include "Sketch.h"

#include "chr/gl/draw/Sprite.h"
#include "chr/path/SplinePath.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace path;

void Sketch::setup()
{
    dotTexture = Texture(
        Texture::ImageRequest("dot_112.png")
            .setFlags(image::FLAGS_TRANSLUCENT)
            .setMipmap(true));

    lineBatch
        .setPrimitive(GL_LINES)
        .setShader(colorShader)
        .setShaderColor(0, 0, 0, 0.67f);

    dotBatch
        .setShader(textureAlphaShader)
        .setShaderColor(1, 0, 0, 0.85f)
        .setTexture(dotTexture);

    // ---

    points.emplace_back(100, 300);
    points.emplace_back(300, 100);
    points.emplace_back(500, 500);
    points.emplace_back(700, 100);
    points.emplace_back(900, 300);

    updateSpline();

    // ---

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // ---

    auto projectionMatrix = glm::ortho(0.0f, windowInfo.width, 0.0f, windowInfo.height);

    Matrix viewMatrix;
    viewMatrix
        .translate(0, windowInfo.height)
        .scale(1, -1);

    State()
        .setShaderMatrix<MVP>(viewMatrix * projectionMatrix)
        .apply();

    lineBatch.flush();
    dotBatch.flush();
}
void Sketch::mouseDragged(int button, float x, float y)
{
    if (selectedIndex != -1)
    {
        points[selectedIndex] = glm::vec2(x, y);
        updateSpline();
    }
}

void Sketch::mousePressed(int button, float x, float y)
{
    selectedIndex = getClosestPointIndex(x, y);
    updateDots();
}

void Sketch::keyDown(int keyCode, int modifiers)
{
    if (selectedIndex != -1)
    {
        float distance = (modifiers & KeyEvent::MODIFIER_SHIFT) ? 10 : 1;

        switch (keyCode)
        {
            case KEY_LEFT:
                points[selectedIndex] += glm::vec2(-distance, 0);
                break;

            case KEY_RIGHT:
                points[selectedIndex] += glm::vec2(+distance, 0);
                break;

            case KEY_UP:
                points[selectedIndex] += glm::vec2(0, -distance);
                break;

            case KEY_DOWN:
                points[selectedIndex] += glm::vec2(0, +distance);
                break;

            default:
                return;
        }

        updateSpline();
    }
}

void Sketch::updateSpline()
{
    SplinePath<glm::vec2> spline;
    spline
        .setType(SplinePath<glm::vec2>::TYPE_BSPLINE)
        .setSamplingTolerance(16);

    for (const auto &point : points)
    {
        spline.add(point);
    }

    drawPolyline(spline.getPolyline());
    updateDots();
}

void Sketch::updateDots()
{
    dotBatch.clear();

    for (int i = 0; i < points.size(); i++)
    {
        auto color = (i == selectedIndex) ? glm::vec4(1, 0, 0, 1) : glm::vec4(0, 0, 0, 1);
        drawDot(points[i], color, 4);
    }
}

int Sketch::getClosestPointIndex(float x, float y)
{
    float closestDistance = 8;
    int closestIndex = -1;

    for (int i = 0; i < points.size(); i++)
    {
        float distance = glm::length(points[i] - glm::vec2(x, y));
        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestIndex = i;
        }
    }

    return closestIndex;
}

void Sketch::drawPolyline(const vector<glm::vec2> &polyline)
{
    lineBatch.clear();

    auto size = polyline.size();
    if (size > 1)
    {
        for (auto i = 0; i < size - 1; i++)
        {
            lineBatch.addVertices(polyline[i], polyline[i + 1]);
        }
    }
}

void Sketch::drawDot(const glm::vec2 &position, const glm::vec4 &color, float radius)
{
    static constexpr float DOT_RADIUS_PIXELS = 56; // Specific to "dot_112.png"

    draw::Sprite()
        .setColor(color)
        .setAnchor(0.5f, 0.5f)
        .append(dotBatch, Matrix()
            .translate(position)
            .scale(radius / DOT_RADIUS_PIXELS));
}
