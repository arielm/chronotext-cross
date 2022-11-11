/*
 * Based on https://learnopengl.com/Lighting/Multiple-lights
 */

attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_coord;
attribute vec3 a_color;

varying vec3 v_frag_pos;
varying vec3 v_normal;
varying vec2 v_coord;
varying vec3 v_color;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;
uniform mat3 u_normal_matrix;

void main()
{
    v_frag_pos = vec3(u_model_matrix * a_position);
    v_normal = u_normal_matrix * a_normal;
    v_coord = a_coord;
    v_color = a_color;

    gl_Position = u_projection_matrix * u_view_matrix * vec4(v_frag_pos, 1.0);
}
