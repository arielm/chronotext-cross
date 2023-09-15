#include "chr/gl/ShaderHelper.h"

#include "chr/Log.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    GLuint ShaderHelper::createShader(GLenum type, const char *source)
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

    tuple<GLuint, GLuint, GLuint> ShaderHelper::loadProgram(const string &vertexShaderSource, const string &fragmentShaderSource)
    {
      GLuint programId = 0;
      GLuint vertexShaderId = 0;
      GLuint fragmentShaderId = 0;

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
              glGetShaderInfoLog(programId, maxLength, &maxLength, &buf[0]); // XXX: glGetProgramInfoLog

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

      return make_tuple(programId, vertexShaderId, fragmentShaderId);
    }

    void ShaderHelper::unloadProgram(GLuint programId, GLuint vertexShaderId, GLuint fragmentShaderId)
    {
      glDetachShader(programId, fragmentShaderId);
      glDeleteShader(fragmentShaderId);

      glDetachShader(programId, vertexShaderId);
      glDeleteShader(vertexShaderId);

      glDeleteProgram(programId);
    }

    // ---

    template <>
    void ShaderHelper::bindAttributes<Vertex<XYZ>>(const ShaderProgram &shader)
    {
      glEnableVertexAttribArray(shader->positionLocation);
      glVertexAttribPointer(shader->positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    template <>
    void ShaderHelper::bindAttributes<Vertex<XYZ.N>>(const ShaderProgram &shader)
    {
      glEnableVertexAttribArray(shader->positionLocation);
      glVertexAttribPointer(shader->positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.N>), 0);

      glEnableVertexAttribArray(shader->normalLocation);
      glVertexAttribPointer(shader->normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.N>), (GLvoid *) sizeof(Vertex<>));
    }

    template <>
    void ShaderHelper::bindAttributes<Vertex<XYZ.UV>>(const ShaderProgram &shader)
    {
      glEnableVertexAttribArray(shader->positionLocation);
      glVertexAttribPointer(shader->positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.UV>), 0);

      glEnableVertexAttribArray(shader->coordLocation);
      glVertexAttribPointer(shader->coordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.UV>), (GLvoid *) sizeof(Vertex<>));
    }

    template <>
    void ShaderHelper::bindAttributes<Vertex<XYZ.RGBA>>(const ShaderProgram &shader)
    {
      glEnableVertexAttribArray(shader->positionLocation);
      glVertexAttribPointer(shader->positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.RGBA>), 0);

      glEnableVertexAttribArray(shader->colorLocation);
      glVertexAttribPointer(shader->colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.RGBA>), (GLvoid *) sizeof(Vertex<XYZ>));
    }

    template <>
    void ShaderHelper::bindAttributes<Vertex<XYZ.N.UV>>(const ShaderProgram &shader)
    {
      glEnableVertexAttribArray(shader->positionLocation);
      glVertexAttribPointer(shader->positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.N.UV>), 0);

      glEnableVertexAttribArray(shader->normalLocation);
      glVertexAttribPointer(shader->normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.N.UV>), (GLvoid *) sizeof(Vertex<>));

      glEnableVertexAttribArray(shader->coordLocation);
      glVertexAttribPointer(shader->coordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.N.UV>), (GLvoid *) sizeof(Vertex<XYZ.N>));
    }

    template <>
    void ShaderHelper::bindAttributes<Vertex<XYZ.N.RGBA>>(const ShaderProgram &shader)
    {
      glEnableVertexAttribArray(shader->positionLocation);
      glVertexAttribPointer(shader->positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.N.RGBA>), 0);

      glEnableVertexAttribArray(shader->normalLocation);
      glVertexAttribPointer(shader->normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.N.RGBA>), (GLvoid *) sizeof(Vertex<>));

      glEnableVertexAttribArray(shader->colorLocation);
      glVertexAttribPointer(shader->colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.N.RGBA>), (GLvoid *) sizeof(Vertex<XYZ.N>));
    }

    template <>
    void ShaderHelper::bindAttributes<Vertex<XYZ.UV.RGBA>>(const ShaderProgram &shader)
    {
      glEnableVertexAttribArray(shader->positionLocation);
      glVertexAttribPointer(shader->positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.UV.RGBA>), 0);

      glEnableVertexAttribArray(shader->coordLocation);
      glVertexAttribPointer(shader->coordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.UV.RGBA>), (GLvoid *) sizeof(Vertex<>));

      glEnableVertexAttribArray(shader->colorLocation);
      glVertexAttribPointer(shader->colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.UV.RGBA>), (GLvoid *) sizeof(Vertex<XYZ.UV>));
    }

    template <>
    void ShaderHelper::bindAttributes<Vertex<XYZ.N.UV.RGBA>>(const ShaderProgram &shader)
    {
      glEnableVertexAttribArray(shader->positionLocation);
      glVertexAttribPointer(shader->positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.N.UV.RGBA>), 0);

      glEnableVertexAttribArray(shader->normalLocation);
      glVertexAttribPointer(shader->normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.N.UV.RGBA>), (GLvoid *) sizeof(Vertex<>));

      glEnableVertexAttribArray(shader->coordLocation);
      glVertexAttribPointer(shader->coordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.N.UV.RGBA>), (GLvoid *) sizeof(Vertex<XYZ.N>));

      glEnableVertexAttribArray(shader->colorLocation);
      glVertexAttribPointer(shader->colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex<XYZ.N.UV.RGBA>), (GLvoid *) sizeof(Vertex<XYZ.N.UV>));
    }

    template <>
    void ShaderHelper::bindAttributes<Vertex<N>>(const ShaderProgram &shader)
    {
      glEnableVertexAttribArray(shader->normalLocation);
      glVertexAttribPointer(shader->normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    template <>
    void ShaderHelper::bindAttributes<Vertex<UV>>(const ShaderProgram &shader)
    {
      glEnableVertexAttribArray(shader->coordLocation);
      glVertexAttribPointer(shader->coordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    template <>
    void ShaderHelper::bindAttributes<Vertex<RGBA>>(const ShaderProgram &shader)
    {
      glEnableVertexAttribArray(shader->colorLocation);
      glVertexAttribPointer(shader->colorLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // ---

    template <>
    void ShaderHelper::unbindAttributes<Vertex<XYZ>>(const ShaderProgram &shader)
    {
      glDisableVertexAttribArray(shader->positionLocation);
    }

    template <>
    void ShaderHelper::unbindAttributes<Vertex<XYZ.N>>(const ShaderProgram &shader)
    {
      glDisableVertexAttribArray(shader->positionLocation);
      glDisableVertexAttribArray(shader->normalLocation);
    }

    template <>
    void ShaderHelper::unbindAttributes<Vertex<XYZ.UV>>(const ShaderProgram &shader)
    {
      glDisableVertexAttribArray(shader->positionLocation);
      glDisableVertexAttribArray(shader->coordLocation);
    }

    template <>
    void ShaderHelper::unbindAttributes<Vertex<XYZ.RGBA>>(const ShaderProgram &shader)
    {
      glDisableVertexAttribArray(shader->positionLocation);
      glDisableVertexAttribArray(shader->colorLocation);
    }

    template <>
    void ShaderHelper::unbindAttributes<Vertex<XYZ.N.UV>>(const ShaderProgram &shader)
    {
      glDisableVertexAttribArray(shader->positionLocation);
      glDisableVertexAttribArray(shader->normalLocation);
      glDisableVertexAttribArray(shader->coordLocation);
    }

    template <>
    void ShaderHelper::unbindAttributes<Vertex<XYZ.N.RGBA>>(const ShaderProgram &shader)
    {
      glDisableVertexAttribArray(shader->positionLocation);
      glDisableVertexAttribArray(shader->normalLocation);
      glDisableVertexAttribArray(shader->colorLocation);
    }

    template <>
    void ShaderHelper::unbindAttributes<Vertex<XYZ.UV.RGBA>>(const ShaderProgram &shader)
    {
      glDisableVertexAttribArray(shader->positionLocation);
      glDisableVertexAttribArray(shader->coordLocation);
      glDisableVertexAttribArray(shader->colorLocation);
    }

    template <>
    void ShaderHelper::unbindAttributes<Vertex<XYZ.N.UV.RGBA>>(const ShaderProgram &shader)
    {
      glDisableVertexAttribArray(shader->positionLocation);
      glDisableVertexAttribArray(shader->normalLocation);
      glDisableVertexAttribArray(shader->coordLocation);
      glDisableVertexAttribArray(shader->colorLocation);
    }

    template <>
    void ShaderHelper::unbindAttributes<Vertex<N>>(const ShaderProgram &shader)
    {
      glDisableVertexAttribArray(shader->normalLocation);
    }

    template <>
    void ShaderHelper::unbindAttributes<Vertex<UV>>(const ShaderProgram &shader)
    {
      glDisableVertexAttribArray(shader->coordLocation);
    }

    template <>
    void ShaderHelper::unbindAttributes<Vertex<RGBA>>(const ShaderProgram &shader)
    {
      glDisableVertexAttribArray(shader->colorLocation);
    }
  }
}
