#pragma once

#include "common_math.h"
#include "TextureManager.h"

class TextureMarble : public TextureManager {
public:
	TextureMarble(float);
	TextureMarble(float, vec3, vec3);
	vec3 getColor(vec3 uvq, Primitive* prim, bool gammaCorrect);
	vec3 getColor(void* params_struct);

private:
	float multiplier;
	vec3  color1;
	vec3  color2;
};