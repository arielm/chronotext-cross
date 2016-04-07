#include "gl/FogTextureAlphaShader.h"

namespace chr
{
  namespace gl
  {
    FogTextureAlphaShader fogTextureAlphaShader;

    const char *FogTextureAlphaShader::vertexShaderSource = R"(
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

    const char *FogTextureAlphaShader::fragmentShaderSource = R"(
    #ifdef GL_ES
      precision mediump float;
    #endif

    uniform sampler2D u_sampler;
    uniform float u_fogDensity;
    uniform vec3 u_fogColor;

    varying vec2 v_coord;
    varying vec4 v_color;

    void main()
    {
      const float LOG2 = 1.442695;
      float z = gl_FragCoord.z / gl_FragCoord.w;
      float fogFactor = exp2(-u_fogDensity * u_fogDensity * z * z * LOG2);

      vec3 rgb = mix(u_fogColor, v_color.rgb, clamp(fogFactor, 0.0, 1.0));
      float alpha = texture2D(u_sampler, v_coord).a;
      gl_FragColor = vec4(rgb, alpha * v_color.a);
    }
    )";

    bool FogTextureAlphaShader::load()
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
