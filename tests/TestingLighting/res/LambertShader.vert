attribute vec3 a_position;
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

  gl_Position = u_mvp_matrix * vec4(a_position, 1);
}
