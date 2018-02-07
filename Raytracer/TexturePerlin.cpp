#include "TexturePerlin.h"
#include <glm\gtc\noise.hpp>

TexturePerlin::TexturePerlin(float multiplier) : TextureManager(), multiplier(multiplier) { 
	// init the random offsets for the perlin functions
	// offset1 = rnd() * 45000;
	// offset2 = rnd() * 45000;
	// offset3 = rnd() * 45000;
};

vec3 TexturePerlin::getColor(vec3 coord, Primitive* prim, bool gammaCorrect) {
	// TODO: this function is now broken. coord doesn't represent anymore the position in space of this point but the triangle's
	// uvs. Transform the uvs to a position in space
	// We're ameliorating the problem by using procTextureFromPointPosition

	float r = pow(glm::perlin(coord * 0.075f + vec3(3462, 363, 224))  * 0.5f + 0.5f, 4.0);
	float g = pow(glm::perlin(coord * 0.075f + vec3(46322, 13, 5745)) * 0.5f + 0.5f, 4.0);
	float b = pow(glm::perlin(coord * 0.075f + vec3(994, 146, 3254))  * 0.5f + 0.5f, 4.0);
	return vec3(r, g, b) * multiplier;
};

vec3 TexturePerlin::getColor(void* params_struct) {
	return vec3(0.0);
};