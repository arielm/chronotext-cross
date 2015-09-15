
#include "CrossSketch.h"

using namespace std;
using namespace chr;

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

#include "stb_image.h"

// ---

static const char *vss = R"(
attribute vec2 a_position;
attribute vec2 a_coord;
attribute vec4 a_color;

uniform mat4 u_mvp_matrix;

varying vec2 v_coord;
varying vec4 v_color;

void main()
{
  v_coord = a_coord;
  v_color = a_color;
  gl_Position = u_mvp_matrix * vec4(a_position, 0, 1);
}
)";

static const char *pss = R"(
#ifdef GL_ES
  precision mediump float;
#endif

uniform sampler2D u_sampler;

varying vec2 v_coord;
varying vec4 v_color;

void main()
{
  gl_FragColor = v_color * texture2D(u_sampler, v_coord);
}
)";

class Sketch : public CrossSketch
{
public:
    virtual ~Sketch() {}

    void setup() final;
    void shutdown() final;
    void draw() final;

    void initBuffers();
    void initTextures();
    void initShaders();

protected:
    GLuint shaderProgram;
    GLint positionLocation;
    GLint coordLocation;
    GLint colorLocation;
    GLint samplerLocation;
    GLint matrixLocation;

    GLuint vboIds[3];
    GLuint textureIds[1];
};

void Sketch::setup()
{
    initBuffers();
    initTextures();
    initShaders();

    // ---

    glEnable(GL_TEXTURE_2D);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::shutdown()
{
    glUseProgram(0);
    glDisableVertexAttribArray(positionLocation);
    glDisableVertexAttribArray(coordLocation);
    glDeleteBuffers(3, vboIds);
    glDeleteTextures(1, textureIds);
}

void Sketch::draw()
{
    glEnableVertexAttribArray(positionLocation);
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(coordLocation);
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
    glVertexAttribPointer(coordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

    GLfloat color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    glVertexAttrib4fv(colorLocation, color);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(samplerLocation, 0);

    // ---

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[2]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

void Sketch::initBuffers()
{
    const GLfloat vertices[] =
    {
        -10, +5, // A
        -10, -5, // B
        +10, -5, // C
        +10, +5, // D
    };

    const GLfloat coords[] =
    {
        0, 0, // A
        0, 1, // B
        1, 1, // C
        1, 0, // D
    };

    const GLushort indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    glGenBuffers(3, vboIds);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 2, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 2, coords, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indices, GL_STATIC_DRAW);
}

void Sketch::initTextures()
{
    fs::path path = "expo67.png";

    stbi_uc *data = nullptr;
    int x, y, comp;

    if (chr::hasMemoryResources())
    {
        auto memoryBuffer = chr::getResourceBuffer(path);

        if (memoryBuffer)
        {
            data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(memoryBuffer->data()), memoryBuffer->size(), &x, &y, &comp, 0);
        }
    }
    else if (chr::hasFileResources())
    {
        auto resPath = chr::getResourcePath(path);
        data = stbi_load(resPath.string().data(), &x, &y, &comp, 0);
    }

    if (data)
    {
        LOGI << "width: " << x << ", height: " << y << ", comp: " << comp << endl;

        glGenTextures(1, textureIds);
        glBindTexture(GL_TEXTURE_2D, textureIds[0]);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        #if !defined(CHR_PLATFORM_DESKTOP)
          glGenerateMipmap(GL_TEXTURE_2D); // FIXME: CRASHES ON DESKTOP
        #endif

        stbi_image_free(data);
    }
    else
    {
        LOGE << "ERROR WHILE LOADING IMAGE" << endl;
    }
}

void Sketch::initShaders()
{
    shaderProgram = makeShaderProgram(vss, pss);
    glUseProgram(shaderProgram);

    positionLocation = glGetAttribLocation(shaderProgram, "a_position");
    coordLocation = glGetAttribLocation(shaderProgram, "a_coord");
    colorLocation = glGetAttribLocation(shaderProgram, "a_color");
    samplerLocation = glGetUniformLocation(shaderProgram, "u_sampler");
    matrixLocation = glGetUniformLocation(shaderProgram, "u_mvp_matrix");
}

// ---

int main(int argc, char** argv)
{
    Sketch sketch;
    sketch.init(800, 600);

    return 0;
}
