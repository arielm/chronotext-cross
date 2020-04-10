#pragma once

#include "chr/gl/Vertex.h"
#include "chr/gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class ShaderHelper
    {
    public:
      static GLuint createShader(GLenum type, const char *source);
      static std::tuple<GLuint, GLuint, GLuint> loadProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
      static void unloadProgram(GLuint programId, GLuint vertexShaderId, GLuint fragmentShaderId);

      template<typename T>
      static void bindAttributes(const ShaderProgram &shaderProgram);

      template<typename T>
      static void unbindAttributes(const ShaderProgram &shaderProgram);
    };
  }
}
