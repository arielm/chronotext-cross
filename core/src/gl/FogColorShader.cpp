#include "FogColorShader.h"

namespace chr
{
  namespace gl
  {
    FogColorShader fogColorShader;
    
    const char *FogColorShader::vertexShaderSource = R"(
    attribute vec3 a_position;
    attribute vec4 a_color;
    uniform mat4 u_matrix;
    varying vec4 v_color;

    void main()
    {
      v_color = a_color;
      gl_Position = u_matrix * vec4(a_position, 1);
    }
    )";

    const char *FogColorShader::fragmentShaderSource = R"(
    #ifdef GL_ES
      precision mediump float;
    #endif

    uniform float u_fogDensity;
    uniform vec3 u_fogColor;
    varying vec4 v_color;

    void main()
    {
      const float LOG2 = 1.442695;
      float z = gl_FragCoord.z / gl_FragCoord.w;
      float fogFactor = exp2(-u_fogDensity * u_fogDensity * z * z * LOG2);

      gl_FragColor = mix(vec4(u_fogColor, 1), v_color, clamp(fogFactor, 0.0, 1.0));
    }
    )";

    bool FogColorShader::load()
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
