#version 300 es

layout(location = 0) in vec3 attr_position;
layout(location = 1) in vec3 attr_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 diffuse;
uniform float ambient;
uniform vec4 specular;
uniform float shininess;
uniform vec4 l_dir;

out vec4 color;

void main() {
    vec4 spec = vec4(0.0);

    vec3 n = vec3(normalize(view * model * vec4(attr_normal, 0.0)));

    float intensity = max(dot(n, vec3(l_dir)), 0.0);

    if (intensity > 0.0) {
        vec3 pos = vec3(view * model * vec4(attr_position, 1.0));
        vec3 eye = normalize(-pos);
        vec3 h = normalize(vec3(l_dir) + eye);

        float intSpec = max(dot(h, n), 0.0);
        spec = specular * pow(intSpec, shininess);
    }

    color = max(intensity * diffuse + spec, ambient * diffuse);

    gl_Position = projection * view * model * vec4(attr_position, 1.0);
}
