
#include "Log.h"
#include "Platform.h"

#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

void report_result(int result)
{
  if (result == 0) {
    printf("Test successful!\n");
  } else {
    printf("Test failed!\n");
  }
#ifdef REPORT_RESULT
  REPORT_RESULT();
#endif
}

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

void draw()
{
  float t = emscripten_get_now() / 1000.0f;
  glm::mat4 mat;
  mat = glm::rotate(mat, t, glm::vec3(0.0f, 0.0f, 1.0f));
  glUniformMatrix4fv(glGetUniformLocation(shader_program, "mat"), 1, GL_FALSE, &mat[0][0]);

  glClearColor(0,0,1,1);
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main()
{
  emscripten_set_canvas_size(256, 256);
  EmscriptenWebGLContextAttributes attr;
  emscripten_webgl_init_context_attributes(&attr);
  attr.alpha = attr.depth = attr.stencil = attr.antialias = attr.preserveDrawingBuffer = attr.preferLowPowerToHighPerformance = attr.failIfMajorPerformanceCaveat = 0;
  attr.enableExtensionsByDefault = 1;
  attr.premultipliedAlpha = 0;
  attr.majorVersion = 1;
  attr.minorVersion = 0;
  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context(0, &attr);
  emscripten_webgl_make_context_current(ctx);

  //

  shader_program = make_shader_program(vss, pss);
  if (shader_program == 0u)
  {
    return -1;
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

  emscripten_set_main_loop(draw, 0, 0);

  return 0;
}
