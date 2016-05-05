attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec4 a_color;

uniform mat4 u_mv_matrix;
uniform mat4 u_projection_matrix;
uniform mat4 u_normal_matrix;

uniform vec3 u_ambient_color;
uniform vec3 u_diffuse_color;
uniform vec3 u_light_dir;

varying vec4 v_color;
varying vec3 v_lighting;

void main()
{
  v_color = a_color;

  vec4 transformedNormal = u_normal_matrix * vec4(a_normal, 1.0);
  float directional = max(dot(transformedNormal.xyz, u_light_dir), 0.0);
  v_lighting = u_ambient_color + (u_diffuse_color * directional);

  gl_Position = u_projection_matrix * u_mv_matrix * vec4(a_position, 1);
}
