
#include "Log.h"
#include "Platform.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

static const char *vertex_shader_text = R"(
attribute vec4 a_position;
attribute vec4 a_color;
varying vec4 v_color;

void main()
{
  v_color = a_color;
  gl_Position = a_position;
}
)";

static const char *fragment_shader_text = R"(
#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_color;

void main()
{
  gl_FragColor = v_color;
}
)";

// ---

static GLuint make_shader(GLenum type, const char* text)
{
    GLuint shader;
    GLint shader_ok;

    shader = glCreateShader(type);
    if (shader != 0)
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
            shader = 0;
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

    glViewport(0, 0, 800, 600);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

    const GLfloat vertices[] =
    {
        -0.5f, +0.5f, // A
        -0.5f, -0.5f, // B
        +0.5f, -0.5f, // C
        +0.5f, +0.5f, // D
    };

    const GLushort indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    GLfloat color[4] = {1.0f, 1.0f, 0.0f, 1.0f};

    GLuint vboIds[2];
    glGenBuffers(2, vboIds);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 2, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indices, GL_STATIC_DRAW);

    //

    GLuint shader_program = make_shader_program(vertex_shader_text, fragment_shader_text);
    if (shader_program == 0u)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glUseProgram(shader_program);
    GLint positionLocation = glGetAttribLocation(shader_program, "a_position");
    GLint colorLocation = glGetAttribLocation(shader_program, "a_color");

    glEnableVertexAttribArray(positionLocation);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        //

        glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttrib4fv(colorLocation, color);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

        //

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDisableVertexAttribArray(positionLocation);
    glDeleteBuffers(2, vboIds);

    glUseProgram(0); // XXX

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
