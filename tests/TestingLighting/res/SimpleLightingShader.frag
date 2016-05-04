#ifdef GL_ES
  precision mediump float;
#endif

varying vec4 v_color;
varying vec3 v_lighting;

void main()
{
  gl_FragColor = vec4(v_color.rgb * v_lighting, v_color.a);
}
