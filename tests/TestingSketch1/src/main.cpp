
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
  gl_FragColor = vec4(1,1,0,1);
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
    GLuint vbo;
};

void Sketch::setup()
{
    shaderProgram = makeShaderProgram(vss, pss);
    glUseProgram(shaderProgram);

    glBindAttribLocation(shaderProgram, 0, "vPosition");

    // ---

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float verts[] = { 0.0, 0.5, 0.0, -0.5f, -0.5f, 0.0, 0.5, -0.5f, 0.0 };
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(0);
}

void Sketch::shutdown()
{
    glDeleteBuffers(1, &vbo);
    glUseProgram(0);
}

void Sketch::draw()
{
    float t = (float)getTime();
    glm::mat4 mat;
    mat = glm::rotate(mat, t, glm::vec3(0.0f, 0.0f, 1.0f)); // FIXME: EMSCRIPTEN CRASH!?
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mat"), 1, GL_FALSE, &mat[0][0]);

    glClearColor(0,1,0,1);
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
