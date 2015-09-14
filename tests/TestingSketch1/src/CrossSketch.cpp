
#include "CrossSketch.h"

using namespace std;

namespace chr
{
  GLuint CrossSketch::makeShader(GLenum type, const char *text)
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

  GLuint CrossSketch::makeShaderProgram(const char *vs_text, const char *fs_text)
  {
    GLuint program = 0u;
    GLint program_ok;
    GLuint vertex_shader = 0u;
    GLuint fragment_shader = 0u;

    vertex_shader = makeShader(GL_VERTEX_SHADER, vs_text);
    if (vertex_shader != 0u)
    {
      fragment_shader = makeShader(GL_FRAGMENT_SHADER, fs_text);
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

  void CrossSketch::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    switch (key)
    {
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;
    }
  }

  void CrossSketch::init(int width, int height)
  {
    if (!initialized)
    {
      if (!glfwInit())
      {
        exit(EXIT_FAILURE);
        return;
      }

      glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

      window = glfwCreateWindow(width, height, "", NULL, NULL);
      if (!window )
      {
        glfwTerminate();
        exit(EXIT_FAILURE);
        return;
      }

      glfwSetKeyCallback(window, keyCallback);

      glfwMakeContextCurrent(window);
      gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

      // ---

      setup();

      while (!glfwWindowShouldClose(window))
      {
        draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
      }

      shutdown();

      glfwTerminate();
      exit(EXIT_SUCCESS);

      // ---

      initialized = true;
    }
  }
}
