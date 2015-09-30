#include "gl/TextureShader.h"

namespace chr
{
  namespace gl
  {
    TextureShader textureShader;

    const char *TextureShader::vertexShaderSource = R"(
    attribute vec2 a_position;
    attribute vec2 a_coord;
    attribute vec4 a_color;

    uniform mat4 u_mvp_matrix;

    varying vec2 v_coord;
    varying vec4 v_color;

    void main()
    {
      v_coord = a_coord;
      v_color = a_color;
      gl_Position = u_mvp_matrix * vec4(a_position, 0, 1);
    }
    )";

    const char *TextureShader::fragmentShaderSource = R"(
    #ifdef GL_ES
      precision mediump float;
    #endif

    uniform sampler2D u_sampler;

    varying vec2 v_coord;
    varying vec4 v_color;

    void main()
    {
      gl_FragColor = v_color * texture2D(u_sampler, v_coord);
    }
    )";

    bool TextureShader::load()
    {
      if (!id)
      {
        ShaderProgram::load(vertexShaderSource, fragmentShaderSource);

        positionLocation = glGetAttribLocation(id, "a_position");
        coordLocation = glGetAttribLocation(id, "a_coord");
        colorLocation = glGetAttribLocation(id, "a_color");
        samplerLocation = glGetUniformLocation(id, "u_sampler");
        matrixLocation = glGetUniformLocation(id, "u_mvp_matrix");
      }

      return bool(id);
    }

    bool TextureShader::use()
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
