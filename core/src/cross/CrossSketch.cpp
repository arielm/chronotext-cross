
#include "CrossSketch.h"

#if defined(CHR_PLATFORM_DESKTOP) || defined(CHR_PLATFORM_EMSCRIPTEN)
  #include "cross/CrossDelegate.h"
#endif

// ---

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

#include "stb_image.h"

// ---

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
      #elif defined(CHR_PLATFORM_EMSCRIPTEN) || defined(CHR_PLATFORM_IOS) || defined(CHR_PLATFORM_ANDROID)
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

#if defined(CHR_PLATFORM_DESKTOP) || defined(CHR_PLATFORM_EMSCRIPTEN)
  void CrossSketch::run(int width, int height, int aaSamples)
  {
      CrossDelegate delegate;
      delegate.run(width, height, aaSamples);
  }
#endif

  double CrossSketch::getElapsedSeconds()
  {
    return timer.getSeconds(); // OUR FrameClock IS NOT SUITED BECAUSE IT PROVIDES A UNIQUE TIME-SAMPLE PER FRAME
  }

  int CrossSketch::getElapsedFrames()
  {
    return frameCount;
  }

  const WindowInfo& CrossSketch::getWindowInfo() const
  {
    return windowInfo;
  }

  FrameClock::Ref CrossSketch::clock() const
  {
    return _clock;
  }

  // ---

  void CrossSketch::performSetup(const WindowInfo &windowInfo)
  {
    this->windowInfo = windowInfo;
    forceResize = true;

    setup();
  }

  void CrossSketch::performResize(const glm::vec2 &size)
  {
    if (forceResize || (size != windowInfo.size))
    {
      windowInfo.size = size;
      forceResize = false;

      resize();
    }
  }

  void CrossSketch::performStart(StartReason reason)
  {
    frameCount = 0;

    timer.start();
    _clock->start();

    start(reason);
  }

  void CrossSketch::performStop(StopReason reason)
  {
    timer.stop();
   _clock->stop();

    stop(reason);
  }

  void CrossSketch::performUpdate()
  {
    update();

    frameCount++;
  }
}
