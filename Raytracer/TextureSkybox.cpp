#include "TextureSkybox.h"
#include <glm\gtc\noise.hpp>

TextureSkybox::TextureSkybox(float multiplier) : TextureManager(), multiplier(multiplier) { };

int TextureSkybox::sampleAxis[3][2] = { { 2, 1 }, { 0, 2 }, { 0, 1 } };

void TextureSkybox::loadTexture(const char * path, int flagFace) {
	FIBITMAP *texture = FreeImage_Load(FIF_PNG, path, NULL);

	// GetBPP returns bits per pixels
	bytesPerPixel = FreeImage_GetBPP(texture) / 8;
	sideWidth = FreeImage_GetWidth(texture);
	// use GetPitch if you want it rounded to the next 32 bits boundary
	unsigned int widthOfBitmapInBytes = FreeImage_GetLine(texture);


	//BYTE* textureBuffer = FreeImage_GetBits(texture);
	//// vector here copies the data, it doesn't refer to the same data pointed by textureData
	//textureData = std::vector<BYTE>(textureBuffer, textureBuffer + sideWidth * sideWidth * bytesPerPixel);


	if (texture) {
		// bitmap successfully loaded!
		FreeImage_Unload(texture);
	}
}

vec3 TextureSkybox::getColor(vec3 direction) {

	//return vec3(pow(r, 2.2f), pow(g, 2.2f), pow(b, 2.2f)) * multiplier;
};
