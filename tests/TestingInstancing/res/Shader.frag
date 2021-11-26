#ifdef GL_ES
    precision highp float;
#endif

uniform sampler2D u_sampler;
uniform float u_shininess;

varying vec3 v_normal;
varying vec4 v_color;
varying vec2 v_coord;
varying vec3 v_surface_to_light;
varying vec3 v_surface_to_view;

void main() {
    vec3 normal = normalize(v_normal);

    vec3 surfaceToLightDirection = normalize(v_surface_to_light);
    vec3 surfaceToViewDirection = normalize(v_surface_to_view);
    vec3 halfVector = normalize(surfaceToLightDirection + surfaceToViewDirection);

    float specular = 0.0;
    float light = dot(normal, surfaceToLightDirection);
    if (light > 0.0) {
        specular = pow(dot(normal, halfVector), u_shininess);
    }

    vec4 color = v_color * texture2D(u_sampler, v_coord);
    gl_FragColor = vec4(color.rgb * light + specular, 1.0);
}
