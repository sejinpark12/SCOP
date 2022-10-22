#version 300 es
precision mediump float;

uniform vec4 diffuse;
uniform float ambient;
uniform vec4 specular;
uniform float shininess;

in vec3 normal;
in vec3 eye;
in vec3 lightDir;

out vec4 outputF;

void main() {
    vec4 spec = vec4(0.0);

    vec3 n = normalize(normal);
    vec3 l = normalize(lightDir);
    vec3 e = normalize(eye);

    float intensity = max(dot(n, l), 0.0);

    if (intensity > 0.0) {
        vec3 h = normalize(l + e);
        float intSpec = max(dot(h, n), 0.0);
        spec = specular * pow(intSpec, shininess);
    }
    outputF = max(intensity * diffuse + spec, ambient * diffuse);
}
