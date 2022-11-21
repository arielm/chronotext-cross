#include "chr/gl/shaders/TextureRectangleShader.h"

namespace chr
{
  namespace gl
  {
    namespace shaders
    {
      static const char *vertexShaderSource = R"(
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

      static const char *fragmentShaderSource = R"(
      #ifdef GL_ES
        precision mediump float;
      #endif

      uniform sampler2DRect u_sampler;

      varying vec2 v_coord;
      varying vec4 v_color;

      void main()
      {
        gl_FragColor = v_color * texture2DRect(u_sampler, v_coord);
      }
      )";

      TextureRectangleShader::TextureRectangleShader()
      :
      ShaderProgram(vertexShaderSource, fragmentShaderSource)
      {}
    }
  }
}
