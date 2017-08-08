#version 330 core

layout(location = 0) in vec4 position;

out vec2 vCoord;

void main() {
	gl_Position = vec4(position.xy, 0, 1);

	vCoord = position.xy * 0.5 + 0.5;
}