#pragma once

#include "Log.h"
#include "Platform.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

namespace chr
{
  class CrossSketch
  {
  public:
    static GLuint makeShader(GLenum type, const char *text);
    static GLuint makeShaderProgram(const char *vs_text, const char *fs_text);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void init(int width, int height);

    virtual void setup();
    virtual void shutdown();
    virtual void draw();

  protected:
    bool initialized = false;
    GLFWwindow* window = nullptr;

    GLuint shaderProgram;
    GLuint vbo;
  };
}
