
#include "CrossSketch.h"

using namespace std;
using namespace chr;

// ---

static const char *vss = R"(
attribute vec2 a_position;
attribute vec4 a_color;

uniform mat4 u_mvp_matrix;
varying vec4 v_color;

void main()
{
  v_color = a_color;
  gl_Position = u_mvp_matrix * vec4(a_position, 0, 1);
}
)";

static const char *pss = R"(
#ifdef GL_ES
  precision mediump float;
#endif

varying vec4 v_color;

void main()
{
  gl_FragColor = v_color;
}
)";

class Sketch : public CrossSketch
{
public:
    virtual ~Sketch() {}

    void setup() final;
    void shutdown() final;
    void draw() final;

    void initShaders();
    void createBuffers();

protected:
    GLuint shaderProgram;
    GLint positionLocation;
    GLint colorLocation;
    GLint matrixLocation;
    GLuint vboIds[2];
};

void Sketch::setup()
{
    createBuffers();
    initShaders();
}

void Sketch::shutdown()
{
    glUseProgram(0);
    glDisableVertexAttribArray(positionLocation);
    glDeleteBuffers(2, vboIds);
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

    glClearColor(0, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

void Sketch::initShaders()
{
    shaderProgram = makeShaderProgram(vss, pss);
    glUseProgram(shaderProgram);

    positionLocation = glGetAttribLocation(shaderProgram, "a_position");
    colorLocation = glGetAttribLocation(shaderProgram, "a_color");
    matrixLocation = glGetUniformLocation(shaderProgram, "u_mvp_matrix");

    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

    GLfloat color[4] = {1.0f, 1.0f, 0.0f, 1.0f};
    glVertexAttrib4fv(colorLocation, color);
}

void Sketch::createBuffers()
{
    const GLfloat vertices[] =
    {
        -10, +5, // A
        -10, -5, // B
        +10, -5, // C
        +10, +5, // D
    };

    const GLushort indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    glGenBuffers(2, vboIds);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 2, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indices, GL_STATIC_DRAW);
}

// ---

int main(int argc, char** argv)
{
    Sketch sketch;
    sketch.init(800, 600);

    return 0;
}
