#pragma once

#include "gl/gl.h"

namespace chr
{
  namespace gl
  {
    class ShaderProgram
    {
    public:
      GLuint id = 0;
      GLuint vertexShaderId;
      GLuint fragmentShaderId;

      GLint mvpMatrixLocation;
      GLint positionLocation;
      GLint colorLocation;
      GLint normalLocation;
      GLint coordLocation;
      GLint samplerLocation;

      static GLuint createShader(GLenum type, const char *source);

      GLuint load(const char *vertexShaderSource, const char *fragmentShaderSource);
      void unload();

      virtual bool use() = 0;

      void applyMVPMatrix(const glm::mat4 &matrix);

      void inline applyColor(float r, float g, float b, float a) { applyColor(glm::vec4(r, g, b, a)); }
      void applyColor(const glm::vec4 &color);
    };
  }
}
