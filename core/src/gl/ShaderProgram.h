#pragma once

#include "gl/gl.h"

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

      GLuint load(std::experimental::string_view vertexShaderSource, std::experimental::string_view fragmentShaderSource);
      void unload();
      bool bind();

      virtual bool load();
      virtual void mapLocations();

      void applyMatrix(const glm::mat4 &matrix);
      void applyColor(float r, float g, float b, float a);
      void applyColor(const glm::vec4 &color);

    protected:
      std::string vertexShaderResourcePath;
      std::string fragmentShaderResourcePath;

      GLuint programId = 0;
      GLuint vertexShaderId;
      GLuint fragmentShaderId;
    };
  }
}
