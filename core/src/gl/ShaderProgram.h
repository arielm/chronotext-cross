#pragma once

#include "gl/gl.h"

#include <unordered_map>
#include <experimental/string_view>

namespace chr
{
  namespace gl
  {
    class ShaderProgram
    {
    public:
      GLuint matrixLocation;
      GLuint positionLocation;
      GLuint colorLocation;
      GLuint normalLocation;
      GLuint coordLocation;
      GLuint samplerLocation;

      static GLuint createShader(GLenum type, const char *source);

      ShaderProgram() = default;
      ShaderProgram(const std::string &vertexShaderResourcePath, const std::string &fragmentShaderResourcePath);

      void unload();
      bool bind();

      virtual bool load();
      virtual void mapLocations();

      void applyMatrix(const glm::mat4 &matrix);
      void applyColor(float r, float g, float b, float a);
      void applyColor(const glm::vec4 &color);

      inline void applyUniform(const std::string &name, int v0) { glUniform1i(getUniformLocation(map_uniform1i, name), v0); }
      inline void applyUniform(const std::string &name, float v0) { glUniform1f(getUniformLocation(map_uniform1f, name), v0); }
      inline void applyUniform(const std::string &name, float v0, float v1) { glUniform2f(getUniformLocation(map_uniform2f, name), v0, v1); }
      inline void applyUniform(const std::string &name, float v0, float v1, float v2) { glUniform3f(getUniformLocation(map_uniform3f, name), v0, v1, v2); }
      inline void applyUniform(const std::string &name, float v0, float v1, float v2, float v3) { glUniform4f(getUniformLocation(map_uniform4f, name), v0, v1, v2, v3); }

    protected:
      std::string vertexShaderResourcePath;
      std::string fragmentShaderResourcePath;

      GLuint programId = 0;
      GLuint vertexShaderId;
      GLuint fragmentShaderId;

      std::unordered_map<std::string, GLuint> map_uniform1i;
      std::unordered_map<std::string, GLuint> map_uniform1f;
      std::unordered_map<std::string, GLuint> map_uniform2f;
      std::unordered_map<std::string, GLuint> map_uniform3f;
      std::unordered_map<std::string, GLuint> map_uniform4f;
//      std::unordered_map<std::string, GLuint> map_uniform2fv;
//      std::unordered_map<std::string, GLuint> map_uniform3fv;
//      std::unordered_map<std::string, GLuint> map_uniform4fv;
//      std::unordered_map<std::string, GLuint> map_uniformMatrix4fv;

      GLuint load(std::experimental::string_view vertexShaderSource, std::experimental::string_view fragmentShaderSource);
      GLuint getUniformLocation(std::unordered_map<std::string, GLuint> &map, const std::string &name);
    };
  }
}
