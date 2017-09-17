#pragma once

#include "common_math.h"
#include "TextureManager.h"

class TexturePerlin : public TextureManager {
public:
	TexturePerlin(float);
	vec3 getColor(vec3 uvq, Primitive* prim);
	vec3 getColor(void* params_struct);

	float multiplier;
};