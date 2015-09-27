#pragma once

#include "gl/Utils.h"

namespace chr
{
  namespace gl
  {
    class ShaderProgram
    {
    public:
        GLuint id;
        GLuint vertexShaderId;
        GLuint fragmentShaderId;

        static GLuint createShader(GLenum type, const char *source);

        GLuint load(const char *vertexShaderSource, const char *fragmentShaderSource);
        void unload();
    };
  }
}
