#include "chr/gl/shaders/TextureLambertShader.h"

namespace chr
{
  namespace gl
  {
    namespace shaders
    {
      static const char *vertexShaderSource = R"(
      attribute vec4 a_position;
      attribute vec3 a_normal;
      attribute vec4 a_color;
      attribute vec2 a_coord;

      uniform mat4 u_mvp_matrix;
      uniform mat3 u_normal_matrix;

      varying vec3 v_normal;
      varying vec4 v_color;
      varying vec2 v_coord;

      void main()
      {
        v_normal = u_normal_matrix * a_normal;
        v_color = a_color;
        v_coord = a_coord;

        gl_Position = u_mvp_matrix * a_position;
      }
      )";

      static const char *fragmentShaderSource = R"(
      #ifdef GL_ES
        precision mediump float;
      #endif

      uniform sampler2D u_sampler;

      varying vec3 v_normal;
      varying vec4 v_color;
      varying vec2 v_coord;

      void main()
      {
        const vec3 L = vec3(0, 0, 1);
        vec3 N = normalize(v_normal);
        float lambert = max(dot(N, L), 0.0);

        vec4 color = v_color * texture2D(u_sampler, v_coord);
        gl_FragColor = vec4(color.rgb * lambert, 1.0);
      }
      )";

      TextureLambertShader::TextureLambertShader()
      :
      ShaderProgram()
      {
        element->vertexShaderSource = vertexShaderSource;
        element->fragmentShaderSource = fragmentShaderSource;
      }
    }
  }
}
