#pragma once

#include "common_math.h"
#include "TextureManager.h"
#include <vector>

#include <FreeImage.h>

typedef unsigned char BYTE;


class Texture2D : public TextureManager {
public:
	Texture2D(const char * path, float multiplier = 0);
	vec3 getColor(vec3 uvq);
	vec3 getColor(void* params_struct);

	float multiplier;
	std::vector<BYTE> textureData;
};