#include "gl/TextureShader.h"

namespace chr
{
  namespace gl
  {
    TextureShader textureShader;

    const char *TextureShader::vertexShaderSource = R"(
    attribute vec3 a_position;
    attribute vec2 a_coord;
    attribute vec4 a_color;

    uniform mat4 u_mvp_matrix;

    varying vec2 v_coord;
    varying vec4 v_color;

    void main()
    {
      v_coord = a_coord;
      v_color = a_color;
      gl_Position = u_mvp_matrix * vec4(a_position, 1);
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

        mvpMatrixLocation = glGetUniformLocation(id, "u_mvp_matrix");
        positionLocation = glGetAttribLocation(id, "a_position");
        colorLocation = glGetAttribLocation(id, "a_color");
        coordLocation = glGetAttribLocation(id, "a_coord");
        samplerLocation = glGetUniformLocation(id, "u_sampler");
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
