
#ifdef GL_ES
  precision highp float;
#endif

/*
 * BASED ON https://github.com/cinder/Cinder/blob/master/samples/Geometry/assets/phong_es2.frag
 */

varying vec3 v_position;
varying vec3 v_normal;
varying vec4 v_color;

void main()
{
    // set diffuse and specular colors
	vec3 cDiffuse = v_color.rgb;
	vec3 cSpecular = vec3(0.3);

  vec3 lightPosition = vec3(0.0);

	// lighting calculations
	vec3 N = normalize(v_normal);
	vec3 L = normalize(lightPosition - v_position.xyz);
	vec3 E = normalize(-v_position.xyz);
	vec3 H = normalize(L + E);

	// Calculate coefficients.
	float phong = max(dot(N, L), 0.0);

	const float kMaterialShininess = 20.0;
	const float kNormalization = (kMaterialShininess + 8.0) / (3.14159265 * 8.0);
	float blinn = pow(max(dot(N, H), 0.0), kMaterialShininess) * kNormalization;

    // diffuse coefficient
	vec3 diffuse = phong * cDiffuse;

	// specular coefficient
  vec3 specular = blinn * cSpecular;

    gl_FragColor = vec4(diffuse + specular, 1);
}
