#include "gl/ShaderProgram.h"

#include "Utils/Utils.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    ShaderProgram::ShaderProgram(const string &vertexShaderResourcePath, const string &fragmentShaderResourcePath)
    :
    vertexShaderResourcePath(vertexShaderResourcePath),
    fragmentShaderResourcePath(fragmentShaderResourcePath)
    {}

    GLuint ShaderProgram::createShader(GLenum type, const char *source)
    {
     GLuint shader = 0;
     shader = glCreateShader(type);

     if (shader != 0)
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

         LOGE << "ERROR: FAILED TO COMPILE " << ((type == GL_FRAGMENT_SHADER) ? "FRAGMENT" : "VERTEX") << " SHADER" << endl;
         LOGE << buf << endl;

         glDeleteShader(shader);
         shader = 0;
       }
     }

     return shader;
    }

    GLuint ShaderProgram::load(experimental::string_view vertexShaderSource, experimental::string_view fragmentShaderSource)
    {
     if (!programId)
     {
       vertexShaderId = createShader(GL_VERTEX_SHADER, vertexShaderSource.data());

       if (vertexShaderId != 0)
       {
         fragmentShaderId = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource.data());

         if (fragmentShaderId != 0)
         {
           programId = glCreateProgram();

           if (programId != 0)
           {
             glAttachShader(programId, vertexShaderId);
             glAttachShader(programId, fragmentShaderId);
             glLinkProgram(programId);

             GLint success;
             glGetProgramiv(programId, GL_LINK_STATUS, &success);

             if (success != GL_TRUE)
             {
               GLint maxLength = 0;
               glGetShaderiv(programId, GL_INFO_LOG_LENGTH, &maxLength);
               string buf(maxLength, 0);
               glGetShaderInfoLog(programId, maxLength, &maxLength, &buf[0]);

               LOGE << "ERROR: FAILED TO LINK SHADER PROGRAM" << endl;
               LOGE << buf << endl;

               glDeleteShader(vertexShaderId);
               vertexShaderId = 0;

               glDeleteShader(fragmentShaderId);
               fragmentShaderId = 0;

               glDeleteProgram(programId);
               programId = 0;
             }
           }
         }
         else
         {
           LOGE << "ERROR: UNABLE TO LOAD FRAGMENT SHADER" << endl;

           glDeleteShader(vertexShaderId);
           vertexShaderId = 0;
         }
       }
       else
       {
         LOGE << "ERROR: UNABLE TO LOAD VERTEX SHADER" << endl;
       }
     }

     return programId;
    }

    void ShaderProgram::unload()
    {
     if (programId)
     {
       glDetachShader(programId, fragmentShaderId);
       glDeleteShader(fragmentShaderId);
       fragmentShaderId = 0;

       glDetachShader(programId, vertexShaderId);
       glDeleteShader(vertexShaderId);
       vertexShaderId = 0;

       glDeleteProgram(programId);
       programId = 0;

       // ---

       map_uniform1i.clear();
       map_uniform1f.clear();
     }
    }

    bool ShaderProgram::bind()
    {
      if (load())
      {
        glUseProgram(programId);
        return true;
      }

      return false;
    }

    bool ShaderProgram::load()
    {
      if (!programId)
      {
        if (!vertexShaderResourcePath.empty() && !fragmentShaderResourcePath.empty())
        {
          load(utils::readTextFromResource<string>(vertexShaderResourcePath), utils::readTextFromResource<string>(fragmentShaderResourcePath));
        }
      }

      return bool(programId);
    }

    void ShaderProgram::mapLocations()
    {
      if (programId)
      {
        matrixLocation = glGetUniformLocation(programId, "u_matrix");
        positionLocation = glGetAttribLocation(programId, "a_position");
        colorLocation = glGetAttribLocation(programId, "a_color");
        normalLocation = glGetAttribLocation(programId, "a_normal");
        coordLocation = glGetAttribLocation(programId, "a_coord");
        samplerLocation = glGetUniformLocation(programId, "u_sampler");
      }
    }

    void ShaderProgram::applyMatrix(const glm::mat4 &matrix)
    {
     glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }

    void ShaderProgram::applyColor(float r, float g, float b, float a)
    {
      glVertexAttrib4f(colorLocation, r, g, b, a);
    }

    void ShaderProgram::applyColor(const glm::vec4 &color)
    {
      glVertexAttrib4fv(colorLocation, &color[0]);
    }

    // ---

    GLuint ShaderProgram::getUniformLocation(std::unordered_map<std::string, GLuint> &map, const std::string &name)
    {
      GLuint location;
      auto found = map_uniform1f.find(name);

      if (found == map_uniform1f.end())
      {
        location = glGetUniformLocation(programId, name.data());
        map_uniform1f.emplace(name, location);
      }
      else
      {
        location = found->second;
      }

      return location;
    }
  }
}
