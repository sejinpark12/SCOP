#version 300 es

layout(location = 0) in vec3 attr_position;
layout(location = 1) in vec3 attr_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 eye;
out vec3 normal;

void main() {
    normal = normalize(vec3(view * model * vec4(attr_normal, 0.0)));
    vec4 pos = view * model * vec4(attr_position, 1.0);
    eye = -pos;

    gl_Position = projection * view * model * vec4(attr_position, 1.0);
}
