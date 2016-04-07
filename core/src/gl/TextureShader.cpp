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

    uniform mat4 u_matrix;

    varying vec2 v_coord;
    varying vec4 v_color;

    void main()
    {
      v_coord = a_coord;
      v_color = a_color;
      gl_Position = u_matrix * vec4(a_position, 1);
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
      if (!programId)
      {
        ShaderProgram::load(vertexShaderSource, fragmentShaderSource);
        mapLocations();
      }

      return bool(programId);
    }
  }
}
