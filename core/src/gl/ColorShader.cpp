#include "gl/ColorShader.h"

namespace chr
{
  namespace gl
  {
    const char *ColorShader::vertexShaderSource = R"(
    attribute vec3 a_position;
    attribute vec4 a_color;
    uniform mat4 u_mvp_matrix;
    varying vec4 v_color;

    void main()
    {
      v_color = a_color;
      gl_Position = u_mvp_matrix * vec4(a_position, 1);
    }
    )";

    const char *ColorShader::fragmentShaderSource = R"(
    #ifdef GL_ES
      precision mediump float;
    #endif

    varying vec4 v_color;

    void main()
    {
      gl_FragColor = v_color;
    }
    )";

    bool ColorShader::load()
    {
      if (!id)
      {
        ShaderProgram::load(vertexShaderSource, fragmentShaderSource);

        positionLocation = glGetAttribLocation(id, "a_position");
        colorLocation = glGetAttribLocation(id, "a_color");
        matrixLocation = glGetUniformLocation(id, "u_mvp_matrix");
      }

      return bool(id);
    }

    bool ColorShader::use()
    {
      if (load())
      {
        glUseProgram(id);
        return true;
      }

      return false;
    }
  }
}
