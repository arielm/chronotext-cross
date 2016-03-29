#include "gl/TextureAlphaShader.h"

namespace chr
{
  namespace gl
  {
    TextureAlphaShader textureAlphaShader;

    const char *TextureAlphaShader::vertexShaderSource = R"(
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

    const char *TextureAlphaShader::fragmentShaderSource = R"(
    #ifdef GL_ES
      precision mediump float;
    #endif

    uniform sampler2D u_sampler;

    varying vec2 v_coord;
    varying vec4 v_color;

    void main()
    {
      float alpha = texture2D(u_sampler, v_coord).a;
      gl_FragColor = vec4(v_color.rgb, alpha * v_color.a);
    }
    )";

    bool TextureAlphaShader::load()
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

    bool TextureAlphaShader::use()
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
