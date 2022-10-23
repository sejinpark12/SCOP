#version 300 es

layout(location = 0) in vec3 attr_position;
layout(location = 1) in vec3 attr_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 l_pos;
uniform vec4 campos;

out vec3 normal;
out vec3 eye;
out vec3 lightDir;

void main() {
    vec4 pos = model * vec4(attr_position, 1.0);

    // 법선행렬 생성
    mat3 m_normal = mat3(transpose(inverse(model)));

    normal = normalize(m_normal * attr_normal);
    lightDir = vec3(l_pos - pos);
    eye = normalize(vec3(campos - pos));

    gl_Position = projection * view * model * vec4(attr_position, 1.0);
}
