#version 330 core

in vec2 vCoord;

uniform sampler2D uDisplayTexture;
uniform int uSamples;

out vec4 Color;

void main(){
	Color = vec4(pow(texture(uDisplayTexture, vCoord).xyz / float(uSamples), vec3(1.0 / 2.2)),  1);	/* gamma corrected */
	//Color = vec4(texture(uDisplayTexture, vCoord).xyz / float(uSamples), 1);
}