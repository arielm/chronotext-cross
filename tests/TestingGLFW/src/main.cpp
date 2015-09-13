
#include "Log.h"
#include "Platform.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// ---

GLuint shader_program;

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
  gl_FragColor = vec4(1,0,0,1);
}
)";

// ---

static GLuint make_shader(GLenum type, const char* text)
{
    GLuint shader = 0u;
    GLint shader_ok;

    shader = glCreateShader(type);
    if (shader != 0u)
    {
        glShaderSource(shader, 1, reinterpret_cast<const GLchar**>(&text), NULL);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
        if (shader_ok != GL_TRUE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            string buf(maxLength, 0);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &buf[0]);

            LOGE << "ERROR: Failed to compile " << ((type == GL_FRAGMENT_SHADER) ? "fragment" : "vertex") << " shader" << endl;
            LOGE << buf << endl;

            glDeleteShader(shader);
            shader = 0u;
        }
    }

    return shader;
}

static GLuint make_shader_program(const char* vs_text, const char* fs_text)
{
    GLuint program = 0u;
    GLint program_ok;
    GLuint vertex_shader = 0u;
    GLuint fragment_shader = 0u;

    vertex_shader = make_shader(GL_VERTEX_SHADER, vs_text);
    if (vertex_shader != 0u)
    {
        fragment_shader = make_shader(GL_FRAGMENT_SHADER, fs_text);
        if (fragment_shader != 0u)
        {
            /* make the program that connect the two shader and link it */
            program = glCreateProgram();
            if (program != 0u)
            {
                /* attach both shader and link */
                glAttachShader(program, vertex_shader);
                glAttachShader(program, fragment_shader);
                glLinkProgram(program);
                glGetProgramiv(program, GL_LINK_STATUS, &program_ok);

                if (program_ok != GL_TRUE)
                {
                    GLint maxLength = 0;
                    glGetShaderiv(program, GL_INFO_LOG_LENGTH, &maxLength);
                    string buf(maxLength, 0);
                    glGetShaderInfoLog(program, maxLength, &maxLength, &buf[0]);

                    LOGE << "ERROR: Failed to link shader program" << endl;
                    LOGE << buf << endl;

                    glDeleteProgram(program);
                    glDeleteShader(fragment_shader);
                    glDeleteShader(vertex_shader);
                    program = 0u;
                }
            }
        }
        else
        {
            LOGE << "ERROR: Unable to load fragment shader" << endl;
            glDeleteShader(vertex_shader);
        }
    }
    else
    {
        LOGE << "ERROR: Unable to load vertex shader" << endl;
    }

    return program;
}

// ---

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch(key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
    }
}

static void error_callback(int error, const char* description)
{
    LOGE << "Error: " << description << endl;
}

int main(int argc, char** argv)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow *window = glfwCreateWindow(800, 600, "***", NULL, NULL);
    if (!window )
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // ---

    shader_program = make_shader_program(vss, pss);
    if (shader_program == 0u)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glUseProgram(shader_program);
    glBindAttribLocation(shader_program, 0, "vPosition");

    //

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float verts[] = { 0.0, 0.5, 0.0, -0.5, -0.5, 0.0, 0.5, -0.5, 0.0 };
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(0);

    int w = 800, h = 600;
    glViewport(0, 0, w, h);

    while (!glfwWindowShouldClose(window))
    {
        float t = (float)glfwGetTime();
        float xs = (float)h / w;
        float ys = 1.0f;
        float mat[] = { cosf(t) * xs, sinf(t) * ys, 0, 0, -sinf(t) * xs, cosf(t) * ys, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "mat"), 1, 0, mat);
        glClearColor(0,0,1,1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);
    glUseProgram(0);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
