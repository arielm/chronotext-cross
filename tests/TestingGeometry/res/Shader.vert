attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec4 a_color;
attribute vec2 a_coord;

uniform mat4 u_mv_matrix;
uniform mat4 u_projection_matrix;
uniform mat3 u_normal_matrix;
uniform vec3 u_eye_position;
uniform vec3 u_light_position;

varying vec3 v_normal;
varying vec4 v_color;
varying vec2 v_coord;
varying vec3 v_surface_to_light;
varying vec3 v_surface_to_view;

void main() {
  v_normal = u_normal_matrix * a_normal;
  v_color = a_color;
  v_coord = a_coord;

  v_surface_to_light = (u_mv_matrix * (vec4(u_light_position, 1.0) - a_position)).xyz;
  v_surface_to_view = (u_mv_matrix * (vec4(u_eye_position, 1.0) - a_position)).xyz;

  gl_Position = u_projection_matrix * u_mv_matrix * a_position;
}
