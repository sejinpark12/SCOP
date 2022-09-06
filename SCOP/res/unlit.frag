#version 300 es
precision mediump float;

//in vec3 vert_color;

layout(location = 0) out vec4 frag_color;

void main() {
	frag_color = vec4(vec3(1.0f, 1.0f, 1.0f), 1.0);
}
