#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>

#include <assert.h>
#include <string>
#include <iostream>

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

GLuint shader_program;

#define PIX_C(x, y) ((x)/256.0f + (y)/256.0f)
#define CLAMP(c) ((c) < 0.f ? 0.f : ((c) > 1.f ? 1.f : (c)))
#define PIX(x, y) CLAMP(PIX_C(x, y))

// ---

static const char *vss = R"(
attribute vec4 vPosition;
uniform mat4 mat; 
varying vec2 texCoord; 

void main()
{
  gl_Position = vPosition;
  texCoord = (vPosition.xy + vec2(1.0)) * vec2(0.5);
}
)";

static const char *pss = R"(
#ifdef GL_ES
precision lowp float;
#endif

varying vec2 texCoord;
uniform vec3 colors[3];
uniform sampler2D tex;

void main()
{
  gl_FragColor = texture2D(tex, texCoord);
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

            cerr << "ERROR: Failed to compile " << ((type == GL_FRAGMENT_SHADER) ? "fragment" : "vertex") << " shader" << endl;
            cerr << buf << endl;

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

                    cerr << "ERROR: Failed to link shader program" << endl;
                    cerr << buf << endl;

                    glDeleteProgram(program);
                    glDeleteShader(fragment_shader);
                    glDeleteShader(vertex_shader);
                    program = 0u;
                }
            }
        }
        else
        {
            cerr << "ERROR: Unable to load fragment shader" << endl;
            glDeleteShader(vertex_shader);
        }
    }
    else
    {
        cerr << "ERROR: Unable to load vertex shader" << endl;
    }

    return program;
}

// ---

void draw()
{
  int w, h, fs;
  emscripten_get_canvas_size(&w, &h, &fs);
  float xs = (float)h / w;
  float ys = 1.0f;
  float mat[] = { xs, 0, 0, 0, 0, ys, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
  glUniformMatrix4fv(glGetUniformLocation(shader_program, "mat"), 1, 0, mat);
  glClearColor(0,0,1,1);
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  unsigned char imageData[256*256*4];
  glReadPixels(0, 0, 256, 256, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
  for(int y = 0; y < 256; ++y)
    for(int x = 0; x < 256; ++x)
    {
      unsigned char red = imageData[(y*256+x)*4];
      float expectedRed = PIX(x, y);
      unsigned char eRed = (unsigned char)(expectedRed * 255.0f);
      assert(fabs((int)eRed - red) <= 2);
    }
  emscripten_cancel_main_loop();
  report_result(0);
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
    return 0;
  }

  glUseProgram(shader_program);
  glBindAttribLocation(shader_program, 0, "vPosition");

  //

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  float verts[] = { -1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, 1 };
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, 0, sizeof(float)*2, 0);
  glEnableVertexAttribArray(0);
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  float texData[256*256];
  for(int y = 0; y < 256; ++y)
    for(int x = 0; x < 256; ++x)
    {
      texData[y*256+x] = PIX(x, y);
    }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 256, 256, 0, GL_LUMINANCE, GL_FLOAT, texData);
  emscripten_set_main_loop(draw, 0, 0);
  return 0;
}
