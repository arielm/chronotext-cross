attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec4 a_color;
attribute vec2 a_coord;
attribute mat4 a_matrix;

uniform mat4 u_mv_matrix;
uniform mat4 u_projection_matrix;
uniform vec3 u_eye_position;
uniform vec3 u_light_position;

varying vec3 v_normal;
varying vec4 v_color;
varying vec2 v_coord;
varying vec3 v_surface_to_light;
varying vec3 v_surface_to_view;

void main() {
  mat4 matrix = u_mv_matrix * a_matrix;

  v_normal = vec3(matrix * vec4(a_normal, 0.0)); // XXX
  v_color = a_color;
  v_coord = a_coord;

  v_surface_to_light = (u_mv_matrix * (vec4(u_light_position, 1.0) - a_position)).xyz;
  v_surface_to_view = (u_mv_matrix * (vec4(u_eye_position, 1.0) - a_position)).xyz;

  gl_Position = u_projection_matrix * matrix * a_position;
}
