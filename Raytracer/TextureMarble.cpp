#include "TextureMarble.h"
#include <glm\gtc\noise.hpp>

TextureMarble::TextureMarble(float multiplier) : TextureManager(), multiplier(multiplier), color1(vec3(1,1,1)), color2(vec3(0,0,0)) {
	// init the random offsets for the perlin functions
	// offset1 = rnd() * 45000;
	// offset2 = rnd() * 45000;
	// offset3 = rnd() * 45000;
};

TextureMarble::TextureMarble(float multiplier, vec3 color1, vec3 color2) : TextureManager(), multiplier(multiplier), color1(color1), color2(color2) {
	// init the random offsets for the perlin functions
	// offset1 = rnd() * 45000;
	// offset2 = rnd() * 45000;
	// offset3 = rnd() * 45000;
};

vec3 TextureMarble::getColor(vec3 coord, Primitive* prim, bool gammaCorrect) {
	// TODO: this function is now broken. coord doesn't represent anymore the position in space of this point but the triangle's
	// uvs. Transform the uvs to a position in space
	// We're ameliorating the problem by using procTextureFromPointPosition



	float p1 = glm::perlin(coord * multiplier + vec3(5432, 555536, 89529));
	float p2 = glm::perlin(vec3(coord.x, coord.y, 0) * multiplier * 0.3f + vec3(125783, 44325, 2662));
	//float p3 = glm::perlin(coord * 0.075f + vec3(3, 52754, 579965));

	float t  = coord.x * multiplier;
	float t2 = coord.z * multiplier;
	float mixer  = (sin(t  + p1 * 2.0f + cos(coord.y * multiplier * 0.5f + p2 * 2.0f) * 2.5f) * 0.5f + 0.5f) * (p2 * 0.5f + 0.5f);
	mixer = pow(mixer, 0.3f);

	vec3 color = mixer * color1 + (1.0f - mixer) * color2;
	return color;
};

vec3 TextureMarble::getColor(void* params_struct) {
	return vec3(0.0);
};