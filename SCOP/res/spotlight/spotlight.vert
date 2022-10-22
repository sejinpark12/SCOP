#version 300 es

layout(location = 0) in vec3 attr_position;
layout(location = 1) in vec3 attr_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 l_pos;

out vec3 normal;
out vec3 eye;
out vec3 lightDir;

void main() {
    vec4 pos = view * model * vec4(attr_position, 1.0);

    normal = normalize(vec3(view * model * vec4(attr_normal, 0.0)));
    lightDir = vec3(l_pos - pos);
    eye = vec3(-pos);

    gl_Position = projection * view * model * vec4(attr_position, 1.0);
}
