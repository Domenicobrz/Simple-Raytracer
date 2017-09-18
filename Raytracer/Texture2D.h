#pragma once

#include "common_math.h"
#include "TextureManager.h"
#include <vector>

#include <FreeImage.h>

typedef unsigned char BYTE;


class Texture2D : public TextureManager {
public:
	Texture2D(const char * path, float multiplier = 1.0f);
	vec3 getColor(vec3 uvq, Primitive* prim);
	vec3 getColor(void* params_struct);

	int textureWidth;
	int textureHeight;
	int bytesPerPixel;
	float multiplier;
	std::vector<BYTE> textureData;
};