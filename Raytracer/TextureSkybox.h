#pragma once

#include "common_math.h"
#include "TextureManager.h"
#include <vector>

#include <FreeImage.h>

typedef unsigned char BYTE;

#define TEXTURESKYBOX_posX 0x00000001
#define TEXTURESKYBOX_posY 0x00000002
#define TEXTURESKYBOX_posZ 0x00000004
		 
#define TEXTURESKYBOX_negX 0x00000008
#define TEXTURESKYBOX_negY 0x00000010
#define TEXTURESKYBOX_negZ 0x00000020

class TextureSkybox : public TextureManager {
public:
	TextureSkybox(float multiplier = 1.0f);

	vec3 getColor(vec3 direction);
	void loadTexture(const char* path, int faceflag);

	int sideWidth;
	int bytesPerPixel;

	float multiplier;
	std::vector<BYTE> textureData_posX;
	std::vector<BYTE> textureData_posY;
	std::vector<BYTE> textureData_posZ;

	std::vector<BYTE> textureData_negX;
	std::vector<BYTE> textureData_negY;
	std::vector<BYTE> textureData_negZ;

	/* 
		The first dimension of the sampleAxis represents the axis of the face pointed by the ray direction 
		the second dimension represents the u and v axis used to calculate the uvs of the face texture

		Algorithm explanation: https://scalibq.wordpress.com/2013/06/23/cubemaps/
	*/
	static int sampleAxis[3][2];
};