/*
 * Based on https://learnopengl.com/Lighting/Multiple-lights
 */

attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_coord;
attribute vec3 a_color;
attribute mat4 a_matrix;

varying vec3 v_frag_pos;
varying vec3 v_normal;
varying vec2 v_coord;
varying vec3 v_color;

uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

void main()
{
    v_frag_pos = vec3(a_matrix * a_position);
    v_normal = vec3(a_matrix * vec4(a_normal, 0.0)); // XXX
    v_coord = a_coord;
    v_color = a_color;

    gl_Position = u_projection_matrix * u_view_matrix * vec4(v_frag_pos, 1.0);
}
