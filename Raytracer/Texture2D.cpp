#include "Texture2D.h"
#include <glm\gtc\noise.hpp>

Texture2D::Texture2D(const char * path, float multiplier) : TextureManager(), multiplier(multiplier) {
	return; 




	FIBITMAP *texture = FreeImage_Load(FIF_PNG, path, NULL);

	/*
	FreeImage_SetOutputMessage(FreeImageErrorHandler);

	if (FreeImage_GetBPP(hImage) != 32)
	{
	FIBITMAP* hOldImage = hImage;
	hImage = FreeImage_ConvertTo32Bits(hOldImage);
	FreeImage_Unload(hOldImage);
	}*/

	unsigned int bitsPerPixel = FreeImage_GetBPP(texture);
	unsigned int imageWidth = FreeImage_GetWidth(texture);
	unsigned int imageHeight = FreeImage_GetHeight(texture);
	// use GetPitch if you want it rounded to the next 32 bits boundary
	unsigned int widthOfBitmapInBytes = FreeImage_GetLine(texture);


	BYTE* textureBuffer = FreeImage_GetBits(texture);
	// vector here copies the data, it doesn't refer to the same data pointed by textureData
	textureData = std::vector<BYTE>(textureBuffer, textureBuffer + imageWidth * imageHeight * (bitsPerPixel / 8));


	if (texture) {
		// bitmap successfully loaded!
		FreeImage_Unload(texture);
	}
};

vec3 Texture2D::getColor(vec3 coord, Primitive* prim) {
	return vec3(coord.x, coord.y, 0.0f) * multiplier;
};

vec3 Texture2D::getColor(void* params_struct) {
	return vec3(0.0);
};