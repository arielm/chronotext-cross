
#ifdef GL_ES
  precision mediump float;
#endif

varying vec3 v_normal;
varying vec4 v_color;

void main()
{
    const vec3 L = vec3(0, 0, 1);
    vec3 N = normalize(v_normal);
    float lambert = max(0.0, dot(N, L));

    gl_FragColor = v_color * vec4(vec3(lambert), 1.0);
}
