attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec4 a_color;

uniform mat4 u_matrix;
uniform mat4 u_n_matrix;

varying vec4 v_color;
varying vec3 v_lighting;

void main()
{
  v_color = a_color;

  vec3 ambientLight = vec3(0.3, 0.3, 0.3);
  vec3 directionalLightColor = vec3(1.0, 1.0, 1.0);
  vec3 directionalVector = vec3(0.0, 0.0, 1.0);

  vec4 transformedNormal = u_n_matrix * vec4(a_normal, 1.0);
  float directional = max(dot(transformedNormal.xyz, directionalVector), 0.0);
  v_lighting = ambientLight + (directionalLightColor * directional);

  gl_Position = u_matrix * vec4(a_position, 1);
}
