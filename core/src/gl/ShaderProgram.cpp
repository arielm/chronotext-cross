#include "gl/ShaderProgram.h"

#include "Log.h"

using namespace std;

namespace chr
{
  namespace gl
  {
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

    GLuint ShaderProgram::load(const char *vertexShaderSource, const char *fragmentShaderSource)
    {
     if (!id)
     {
       vertexShaderId = createShader(GL_VERTEX_SHADER, vertexShaderSource);

       if (vertexShaderId != 0)
       {
         fragmentShaderId = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

         if (fragmentShaderId != 0)
         {
           id = glCreateProgram();

           if (id != 0)
           {
             glAttachShader(id, vertexShaderId);
             glAttachShader(id, fragmentShaderId);
             glLinkProgram(id);

             GLint success;
             glGetProgramiv(id, GL_LINK_STATUS, &success);

             if (success != GL_TRUE)
             {
               GLint maxLength = 0;
               glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
               string buf(maxLength, 0);
               glGetShaderInfoLog(id, maxLength, &maxLength, &buf[0]);

               LOGE << "ERROR: FAILED TO LINK SHADER PROGRAM" << endl;
               LOGE << buf << endl;

               glDeleteShader(vertexShaderId);
               vertexShaderId = 0;

               glDeleteShader(fragmentShaderId);
               fragmentShaderId = 0;

               glDeleteProgram(id);
               id = 0;
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

     return id;
    }

    void ShaderProgram::unload()
    {
     if (id)
     {
       glDetachShader(id, fragmentShaderId);
       glDeleteShader(fragmentShaderId);
       fragmentShaderId = 0;

       glDetachShader(id, vertexShaderId);
       glDeleteShader(vertexShaderId);
       vertexShaderId = 0;

       glDeleteProgram(id);
       id = 0;
     }
    }

    void ShaderProgram::applyMVPMatrix(const glm::mat4 &matrix)
    {
     glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }

    void ShaderProgram::applyColor(const glm::vec4 &color)
    {
      glVertexAttrib4fv(colorLocation, &color[0]);
    }
  }
}
