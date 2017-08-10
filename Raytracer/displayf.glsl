#version 330 core

in vec2 vCoord;

uniform sampler2D uDisplayTexture;
uniform int uSamples;

out vec4 Color;

void main(){
	Color = vec4(sqrt(texture(uDisplayTexture, vCoord).xyz / float(uSamples)), 1);
}