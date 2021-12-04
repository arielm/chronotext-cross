#pragma once

#include "chr/InputSource.h"
#include "chr/gl.h"

#include <map>
#include <vector>

namespace chr
{
  namespace gl
  {
    enum
    {
      MV = 0,
      MVP = 1,
      PROJECTION = 2,
      MODEL = 3,
      VIEW = 4
    };

    enum
    {
      NORMAL = 0
    };

    namespace shader
    {
      struct Element
      {
        int useCount = 0;

        std::string vertexShaderSource;
        std::string fragmentShaderSource;

        GLuint programId;
        GLuint vertexShaderId;
        GLuint fragmentShaderId;

        GLuint mvMatrixLocation;
        GLuint mvpMatrixLocation;
        GLuint projectionMatrixLocation;
        GLuint modelMatrixLocation;
        GLuint viewMatrixLocation;
        GLuint normalMatrixLocation;
        GLuint positionLocation;
        GLuint colorLocation;
        GLuint normalLocation;
        GLuint coordLocation;
        GLuint matrixLocation;
        GLuint samplerLocation;

        std::map<std::string, GLuint> map_uniform;
      };
    }

    class ShaderProgram
    {
    public:
      int id = -1;
      shader::Element *element = nullptr;

      ShaderProgram() = default;
      ShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
      ShaderProgram(const InputSource &vertexShaderSource, const InputSource &fragmentShaderSource);
      ShaderProgram(const ShaderProgram &other);
      ShaderProgram& operator=(const ShaderProgram &other);
      bool operator==(const ShaderProgram& other) const;

      ~ShaderProgram();

      shader::Element* operator-> () const
      {
        return element;
      }

      bool bind();
      void unbind();
      void unload();
      void purge();

      GLuint getUniformLocation(const std::string &name);

      void applyColor(float r, float g, float b, float a);
      void applyColor(const glm::vec4 &color);

      template<int T = MVP>
      void applyMatrix(const glm::mat4 &matrix);

      template<int T>
      void applyMatrix(const glm::mat3 &matrix);

      void applyUniform(const std::string &name, const std::vector<int> &v);
      void applyUniform(const std::string &name, const std::vector<float> &v);

      inline void applyUniform(const std::string &name, int v0) { glUniform1i(getUniformLocation(name), v0); }
      inline void applyUniform(const std::string &name, int v0, int v1) { glUniform2i(getUniformLocation(name), v0, v1); }
      inline void applyUniform(const std::string &name, int v0, int v1, int v2) { glUniform3i(getUniformLocation(name), v0, v1, v2); }
      inline void applyUniform(const std::string &name, int v0, int v1, int v2, int v3) { glUniform4i(getUniformLocation(name), v0, v1, v2, v3); }
      inline void applyUniform(const std::string &name, float v0) { glUniform1f(getUniformLocation(name), v0); }
      inline void applyUniform(const std::string &name, float v0, float v1) { glUniform2f(getUniformLocation(name), v0, v1); }
      inline void applyUniform(const std::string &name, float v0, float v1, float v2) { glUniform3f(getUniformLocation(name), v0, v1, v2); }
      inline void applyUniform(const std::string &name, float v0, float v1, float v2, float v3) { glUniform4f(getUniformLocation(name), v0, v1, v2, v3); }
      inline void applyUniform(const std::string &name, const glm::ivec2 &v) { glUniform2iv(getUniformLocation(name), 1, &v[0]); }
      inline void applyUniform(const std::string &name, const glm::ivec3 &v) { glUniform3iv(getUniformLocation(name), 1, &v[0]); }
      inline void applyUniform(const std::string &name, const glm::ivec4 &v) { glUniform4iv(getUniformLocation(name), 1, &v[0]); }
      inline void applyUniform(const std::string &name, const glm::vec2 &v) { glUniform2fv(getUniformLocation(name), 1, &v[0]); }
      inline void applyUniform(const std::string &name, const glm::vec3 &v) { glUniform3fv(getUniformLocation(name), 1, &v[0]); }
      inline void applyUniform(const std::string &name, const glm::vec4 &v) { glUniform4fv(getUniformLocation(name), 1, &v[0]); }
      inline void applyUniform(const std::string &name, const glm::mat3 &m) { glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &m[0][0]); }
      inline void applyUniform(const std::string &name, const glm::mat4 &m) { glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &m[0][0]); }

    protected:
      static int usageCounter;

      bool load();
    };
  }
}
