
#include "CrossSketch.h"

using namespace std;
using namespace chr;

// ---

static const char *vss = R"(
attribute vec4 vPosition;
uniform mat4 mat;

void main()
{
  gl_Position = mat * vPosition;
}
)";

static const char *pss = R"(
#ifdef GL_ES
precision lowp float;
#endif

void main()
{
  gl_FragColor = vec4(1, 1, 0, 1);
}
)";

class Sketch : public CrossSketch
{
public:
    virtual ~Sketch() {}

    void setup() final;
    void shutdown() final;
    void draw() final;

protected:
    GLuint shaderProgram;
    GLint matrixLocation;
    GLuint vbo;
};

void Sketch::setup()
{
    shaderProgram = makeShaderProgram(vss, pss);
    glUseProgram(shaderProgram);

    glBindAttribLocation(shaderProgram, 0, "vPosition");
    matrixLocation = glGetUniformLocation(shaderProgram, "mat");

    // ---

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float verts[] = { 0.0, 5.0f, 0.0, -5.0f, -5.0f, 0.0, 5.0f, -5.0f, 0.0 };
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void Sketch::shutdown()
{
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &vbo);
}

void Sketch::draw()
{
    glm::mat4 projectionMatrix = glm::perspective(60 * D2R, screenSize.x / screenSize.y, 0.1f, 100.0f);

    glm::mat4 modelViewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)); // SCALE
    modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0, 0, -30)); // DISTANCE
    modelViewMatrix = glm::rotate(modelViewMatrix, -15 * D2R, glm::vec3(1, 0, 0)); // ELEVATION
    modelViewMatrix = glm::rotate(modelViewMatrix, (float)getTime(), glm::vec3(0, 1, 0)); // AZIMUTH

    glm::mat4 mvp = projectionMatrix * modelViewMatrix;
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &mvp[0][0]);

    // ---
    
    glClearColor(0, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

// ---

int main(int argc, char** argv)
{
    Sketch sketch;
    sketch.init(800, 600);

    return 0;
}
