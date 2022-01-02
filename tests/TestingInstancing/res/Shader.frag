/*
 * Based on http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading
 */

#ifdef GL_ES
    precision highp float;
#endif

uniform sampler2D u_sampler;
uniform vec3 u_light_position;
uniform vec3 u_light_color;
uniform float u_light_intensity;
uniform vec3 u_ambient_color;
uniform vec3 u_diffuse_color;
uniform vec3 u_specular_color;
uniform float u_shininess;
uniform int u_has_texture;
uniform int u_has_color;

varying vec4 v_color;
varying vec2 v_coord;
varying vec3 v_position_worldspace;
varying vec3 v_normal_cameraspace;
varying vec3 v_eye_direction_cameraspace;
varying vec3 v_light_direction_cameraspace;

void main()
{
    // Normal of the computed fragment, in camera space
    vec3 n = normalize(v_normal_cameraspace);
    // Direction of the light (from the fragment to the light)
    vec3 l = normalize(v_light_direction_cameraspace);
    // Cosine of the angle between the normal and the light direction,
    // clamped above 0
    //  - light is at the vertical of the triangle -> 1
    //  - light is perpendicular to the triangle -> 0
    //  - light is behind the triangle -> 0
    float cosTheta = clamp(dot(n, l), 0.0, 1.0);

    // Eye vector (towards the camera)
    vec3 E = normalize(v_eye_direction_cameraspace);
    // Direction in which the triangle reflects the light
    vec3 R = reflect(-l, n);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha = clamp(dot(E, R), 0.0, 1.0);

    vec4 color = (u_has_color == 1) ? v_color : vec4(u_diffuse_color, 1);
    if (u_has_texture == 1)
    {
        color *= texture2D(u_sampler, v_coord);
    }

    gl_FragColor =
        vec4(u_ambient_color +
        color.rgb * u_light_color * u_light_intensity * cosTheta +
        u_specular_color * u_light_color * u_light_intensity * pow(cosAlpha, u_shininess), 1.0);
}
