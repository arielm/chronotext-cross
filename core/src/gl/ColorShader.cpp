#include "gl/ColorShader.h"

namespace chr
{
  namespace gl
  {
    ColorShader colorShader;
    
    static const char *vertexShaderSource = R"(
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

    static const char *fragmentShaderSource = R"(
    #ifdef GL_ES
      precision mediump float;
    #endif

    varying vec4 v_color;

    void main()
    {
      gl_FragColor = v_color;
    }
    )";

    bool ColorShader::load()
    {
      if (!programId)
      {
        reload(vertexShaderSource, fragmentShaderSource);
        mapLocations();
      }

      return bool(programId);
    }
  }
}
