#include "gl/TextureAlphaShader.h"

namespace chr
{
  namespace gl
  {
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

    void TextureAlphaShader::load()
    {
      ShaderProgram::load(vertexShaderSource, fragmentShaderSource);

      positionLocation = glGetAttribLocation(id, "a_position");
      coordLocation = glGetAttribLocation(id, "a_coord");
      colorLocation = glGetAttribLocation(id, "a_color");
      samplerLocation = glGetUniformLocation(id, "u_sampler");
      matrixLocation = glGetUniformLocation(id, "u_mvp_matrix");
    }
  }
}
