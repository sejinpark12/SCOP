#version 300 es
precision mediump float;

uniform vec4 diffuse;
uniform float ambient;
uniform vec4 specular;
uniform float shininess;

uniform vec4 l_pos, l_spotDir;
uniform float l_spotCutOff;

in vec3 normal;
in vec3 eye;
in vec3 lightDir;

out vec4 outputF;

void main() {
    float intensity = 0.0;
    vec4 spec = vec4(0.0);

    vec3 ld = normalize(lightDir);
    vec3 sd = normalize(vec3(-l_spotDir));

    if (dot(sd, ld) > l_spotCutOff) {
        vec3 n = normalize(normal);
        intensity = max(dot(n, ld), 0.0);

        if (intensity > 0.0) {
            vec3 e = normalize(eye);
            vec3 h = normalize(ld + e);
            float intSpec = max(dot(h, n), 0.0);
            spec = specular * pow(intSpec, shininess);

        }

    }
    outputF = max(intensity * diffuse + spec, ambient * diffuse);
}
