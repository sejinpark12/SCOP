#version 300 es
precision mediump float;

uniform vec4 diffuse;
uniform float ambient;
uniform vec4 specular;
uniform float shininess;
uniform vec4 l_dir;

in vec3 normal;
in vec4 eye;

out vec4 outputF;

void main() {
    vec4 spec = vec4(0.0);

    vec3 n = normalize(normal);
    vec3 e = normalize(vec3(eye));
    vec3 light_dir = vec3(l_dir);

    float intensity = max(dot(n, light_dir), 0.0);

    if (intensity > 0.0) {
        vec3 h = normalize(light_dir + e);
        float intSpec = max(dot(h, n), 0.0);
        spec = specular * pow(intSpec, shininess);
    }
    outputF = max(intensity * diffuse + spec, ambient * diffuse);
}
