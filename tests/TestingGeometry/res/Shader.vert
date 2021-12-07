/*
 * Based on http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading
 */

attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec4 a_color;
attribute vec2 a_coord;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;
uniform mat3 u_normal_matrix;
uniform vec3 u_light_position;

varying vec4 v_color;
varying vec2 v_coord;
varying vec3 v_position_worldspace;
varying vec3 v_normal_cameraspace;
varying vec3 v_eye_direction_cameraspace;
varying vec3 v_light_direction_cameraspace;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * a_position;

	// Position of the vertex, in worldspace : M * position
	v_position_worldspace = (u_model_matrix * a_position).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = (u_view_matrix * u_model_matrix * a_position).xyz;
	v_eye_direction_cameraspace = vec3(0.0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 lightPosition_cameraspace = (u_view_matrix * vec4(u_light_position, 1.0)).xyz;
	v_light_direction_cameraspace = lightPosition_cameraspace + v_eye_direction_cameraspace;

	// Normal of the the vertex, in camera space
	v_normal_cameraspace = u_normal_matrix * a_normal;

    v_color = a_color;
    v_coord = a_coord;
}
