#include "gl/Utils.h"
#include "Log.h"
#include "MemoryBuffer.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include "stb_image.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    GLuint makeShader(GLenum type, const char *source)
    {
      GLuint shader = 0u;
      shader = glCreateShader(type);

      if (shader != 0u)
      {
        glShaderSource(shader, 1, reinterpret_cast<const GLchar**>(&source), NULL);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (success != GL_TRUE)
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

    GLuint makeShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource)
    {
      GLuint program = 0u;
      GLuint vertex_shader = 0u;
      GLuint fragment_shader = 0u;
      vertex_shader = makeShader(GL_VERTEX_SHADER, vertexShaderSource);

      if (vertex_shader != 0u)
      {
        fragment_shader = makeShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        if (fragment_shader != 0u)
        {
          program = glCreateProgram();

          if (program != 0u)
          {
            glAttachShader(program, vertex_shader);
            glAttachShader(program, fragment_shader);
            glLinkProgram(program);

            GLint success;
            glGetProgramiv(program, GL_LINK_STATUS, &success);

            if (success != GL_TRUE)
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

    GLuint loadTexture(const fs::path &relativePath, bool forceAlpha)
    {
      GLuint texture = 0u;

      stbi_uc *data = nullptr;
      int x, y, comp;

      if (hasMemoryResources())
      {
        auto memoryBuffer = getResourceBuffer(relativePath);

        if (memoryBuffer)
        {
          data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(memoryBuffer->data()), memoryBuffer->size(), &x, &y, &comp, 0);
        }
      }
      else if (hasFileResources())
      {
        auto resPath = getResourcePath(relativePath);
        data = stbi_load(resPath.string().data(), &x, &y, &comp, 0);
      }

      if (data)
      {
        GLenum format = 0;

        switch (comp)
        {
          case 1:
            format = GL_ALPHA;
            break;

          case 3:
            format = GL_RGB;
            break;

          case 4:
            format = GL_RGBA;
            break;
        }

        if (format)
        {
          glGenTextures(1, &texture);
          glBindTexture(GL_TEXTURE_2D, texture);

          if (forceAlpha && (format != GL_ALPHA))
          {
            int size = x * y;
            int offset = comp - 1;

            auto converted = make_unique<uint8_t[]>(size);
            auto buffer = converted.get();

            for (auto i = 0; i < size; i++, offset += comp)
            {
              *buffer++ = data[offset];
            }

            uploadTextureData(GL_ALPHA, x, y, converted.get());
          }
          else
          {
            uploadTextureData(format, x, y, data);
          }
        }
        else
        {
          LOGE << "UNSUPPORTED IMAGE FORMAT" << endl;
        }

        stbi_image_free(data);
      }
      else
      {
        LOGE << "ERROR WHILE LOADING IMAGE" << endl;
      }

      return texture;
    }

    void uploadTextureData(GLenum format, GLsizei width, GLsizei height, const GLvoid *data)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

      #if defined(CHR_PLATFORM_DESKTOP)
        glHint(0x8192, GL_NICEST); // GL_GENERATE_MIPMAP_HINT
        glTexParameteri(GL_TEXTURE_2D, 0x8191, GL_TRUE); // GL_GENERATE_MIPMAP
      #endif

      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

      #if defined(CHR_PLATFORM_DESKTOP)
        glTexParameteri(GL_TEXTURE_2D, 0x8191, GL_FALSE); // GL_GENERATE_MIPMAP
      #elif defined(CHR_PLATFORM_EMSCRIPTEN) || defined(CHR_PLATFORM_IOS) || defined(CHR_PLATFORM_ANDROID)
        glGenerateMipmap(GL_TEXTURE_2D);
      #endif
    }
  }
}
