/*
 * Based on https://learnopengl.com/Lighting/Multiple-lights
 */

#ifdef GL_ES
    precision mediump float;
#endif

struct Material
{
    int has_dir_light;
    int point_light_count;
    int has_color;
    int has_texture;
    sampler2D texture;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PointLight
{
    vec3 position;
    vec3 color;
    float intensity;
};

varying vec3 v_frag_pos;
varying vec3 v_normal;
varying vec2 v_coord;
varying vec3 v_color;

uniform vec3 u_view_pos;
uniform DirLight u_dir_light;
uniform PointLight u_point_lights[3];
uniform Material u_material;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-light.direction);

    float diff = max(dot(normal, light_dir), 0.0);

    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.shininess);

    vec3 ambient = u_material.ambient;
    vec3 diffuse = ((u_material.has_color == 1) ? v_color : u_material.diffuse) * diff * light.color * light.intensity;
    vec3 specular = u_material.specular * spec * light.color * light.intensity;

    if (u_material.has_texture == 1)
    {
        vec3 sample = vec3(texture2D(u_material.texture, v_coord));
        ambient *= sample;
        diffuse *= sample;
    }

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position - v_frag_pos);

    float diff = max(dot(normal, light_dir), 0.0);

    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.shininess);

    vec3 ambient = u_material.ambient;
    vec3 diffuse = ((u_material.has_color == 1) ? v_color : u_material.diffuse) * diff * light.color * light.intensity;
    vec3 specular = u_material.specular * spec * light.color * light.intensity;

    if (u_material.has_texture == 1)
    {
        vec3 sample = vec3(texture2D(u_material.texture, v_coord));
        ambient *= sample;
        diffuse *= sample;
    }

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(v_normal);
    vec3 view_dir = normalize(u_view_pos - v_frag_pos);

    vec3 result = vec3(0);

    if (u_material.has_dir_light == 1)
    {
        result += calcDirLight(u_dir_light, norm, view_dir);
    }

    if (u_material.point_light_count == 1)
    {
        result += calcPointLight(u_point_lights[0], norm, view_dir);
    }
    else if (u_material.point_light_count == 2)
    {
        result += calcPointLight(u_point_lights[0], norm, view_dir);
        result += calcPointLight(u_point_lights[1], norm, view_dir);
    }
    else if (u_material.point_light_count == 3)
    {
        result += calcPointLight(u_point_lights[0], norm, view_dir);
        result += calcPointLight(u_point_lights[1], norm, view_dir);
        result += calcPointLight(u_point_lights[2], norm, view_dir);
    }

    gl_FragColor = vec4(result, 1.0);
}
