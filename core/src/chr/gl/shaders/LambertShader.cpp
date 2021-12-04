#include "chr/gl/shaders/LambertShader.h"

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

      uniform mat4 u_mvp_matrix;
      uniform mat3 u_normal_matrix;

      varying vec3 v_normal;
      varying vec4 v_color;

      void main()
      {
        v_normal = u_normal_matrix * a_normal;
        v_color = a_color;

        gl_Position = u_mvp_matrix * a_position;
      }
      )";

      static const char *fragmentShaderSource = R"(
      #ifdef GL_ES
        precision mediump float;
      #endif

      varying vec3 v_normal;
      varying vec4 v_color;

      void main()
      {
        const vec3 L = vec3(0, 0, 1);
        vec3 N = normalize(v_normal);
        float lambert = max(dot(N, L), 0.0);

        gl_FragColor = vec4(v_color.rgb * lambert, 1.0);
      }
      )";

      LambertShader::LambertShader()
      :
      ShaderProgram(vertexShaderSource, fragmentShaderSource)
      {}
    }
  }
}
