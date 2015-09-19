
#include "CrossSketch.h"

using namespace std;

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

#include "stb_image.h"

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

  GLuint CrossSketch::loadTexture(const fs::path &relativePath)
  {
    GLuint texture = 0u;

    stbi_uc *data = nullptr;
    int x, y, comp;

    if (chr::hasMemoryResources())
    {
      auto memoryBuffer = chr::getResourceBuffer(relativePath);

      if (memoryBuffer)
      {
        data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(memoryBuffer->data()), memoryBuffer->size(), &x, &y, &comp, 0);
      }
    }
    else if (chr::hasFileResources())
    {
      auto resPath = chr::getResourcePath(relativePath);
      data = stbi_load(resPath.string().data(), &x, &y, &comp, 0);
    }

    if (data)
    {
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

      #if defined(CHR_PLATFORM_DESKTOP)
        glHint(0x8192, GL_NICEST); // GL_GENERATE_MIPMAP_HINT
        glTexParameteri(GL_TEXTURE_2D, 0x8191, GL_TRUE); // GL_GENERATE_MIPMAP
      #endif

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

      #if defined(CHR_PLATFORM_DESKTOP)
        glTexParameteri(GL_TEXTURE_2D, 0x8191, GL_FALSE); // GL_GENERATE_MIPMAP
      #elif defined(CHR_PLATFORM_EMSCRIPTEN)
        glGenerateMipmap(GL_TEXTURE_2D);
      #endif

      stbi_image_free(data);
    }
    else
    {
      LOGE << "ERROR WHILE LOADING IMAGE" << endl;
    }

    return texture;
  }

#if defined(CHR_PLATFORM_DESKTOP)
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

      screenSize.x = width;
      screenSize.y = height;
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

  double CrossSketch::getTime()
  {
    return glfwGetTime();
  }
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  void CrossSketch::performDraw(void *data)
  {
    reinterpret_cast<CrossSketch*>(data)->draw();
  }

  void CrossSketch::init(int width, int height)
  {
    if (!initialized)
    {
      emscripten_set_canvas_size(width, height);

      EmscriptenWebGLContextAttributes attr;
      emscripten_webgl_init_context_attributes(&attr);
      attr.alpha = attr.depth = attr.stencil = attr.preserveDrawingBuffer = attr.preferLowPowerToHighPerformance = attr.failIfMajorPerformanceCaveat = 0;
      attr.enableExtensionsByDefault = 1;
      attr.antialias = 1;
      attr.premultipliedAlpha = 0;
      attr.majorVersion = 1;
      attr.minorVersion = 0;
      EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context(0, &attr);
      emscripten_webgl_make_context_current(ctx);

      // ---

      screenSize.x = width;
      screenSize.y = height;
      setup();

      emscripten_set_main_loop_arg(performDraw, this, 0, 1);

      // ---

      initialized = true;
    }
  }

  double CrossSketch::getTime()
  {
    return emscripten_get_now() / 1000.0;
  }
#elif defined(CHR_PLATFORM_IOS)
  double CrossSketch::getTime()
  {
    return 0; // TODO
  }
#endif

  // ---

  void CrossSketch::performSetup(const WindowInfo &windowInfo) // TODO
  {
    LOGI << "SETUP - " << windowInfo << endl;
  }

  void CrossSketch::performResize(const glm::vec2 &size) // TODO
  {
    LOGI << "RESIZE - [" << size.x << ", " << size.y << "]" << endl;
  }

  void CrossSketch::performStart(StartReason reason) // TODO
  {}

  void CrossSketch::performStop(StopReason reason) // TODO
  {}

  void CrossSketch::performUpdate() // TODO
  {}
}
