#include "Texture2D.h"
#include <glm\gtc\noise.hpp>

Texture2D::Texture2D(const char * path, float multiplier) : TextureManager(), multiplier(multiplier) {
	FIBITMAP *texture = FreeImage_Load(FIF_PNG, path, NULL);

	/*
	FreeImage_SetOutputMessage(FreeImageErrorHandler);


	// below code is implementation ready
	// below code is implementation ready
	// below code is implementation ready

	if (FreeImage_GetBPP(texture) != 32)
	{
		FIBITMAP* hOldImage = texture;
		texture = FreeImage_ConvertTo32Bits(hOldImage);
		FreeImage_Unload(hOldImage);
	}
	*/

				    // GetBPP returns bits per pixels
	bytesPerPixel = FreeImage_GetBPP(texture) / 8;
	textureWidth = FreeImage_GetWidth(texture);
	textureHeight = FreeImage_GetHeight(texture);
	// use GetPitch if you want it rounded to the next 32 bits boundary
	unsigned int widthOfBitmapInBytes = FreeImage_GetLine(texture);


	BYTE* textureBuffer = FreeImage_GetBits(texture);
	// vector here copies the data, it doesn't refer to the same data pointed by textureData
	textureData = std::vector<BYTE>(textureBuffer, textureBuffer + textureWidth * textureHeight * bytesPerPixel);


	if (texture) {
		// bitmap successfully loaded!
		FreeImage_Unload(texture);
	}
};

vec3 Texture2D::getColor(vec3 coord, Primitive* prim) {
	
	vec2 uv = prim->getInterpolatedUV(vec2(coord.x, coord.y));
	// returned interpolated uvs may get up to 1.0f, so they need to be clamped
	// this is not the fault of getInterpolatedUV(...)
	// in another scenario, where we're interpolating different vertex attributes it could be correct to
	// return exactly 1.0, which would mean only one vertex is contributing to the interpolated value.
	// This is the reason we're clamping uvs here since our texture routine requires to have uvs smaller than 1.0f
	uv.s = min(uv.s, 0.999999f);
	uv.t = min(uv.t, 0.999999f);

	int textureX = (int)(uv.s * (float)textureWidth);
	int textureY = (int)(uv.t * (float)textureHeight);
	
	int pixelIndex = (textureY * textureWidth + textureX) * bytesPerPixel;


#ifdef DEBUG
	if (pixelIndex > textureWidth * textureHeight * bytesPerPixel) {
		printf("index out of bounds: %d, max is: %d", pixelIndex, textureWidth * textureHeight * bytesPerPixel + 2);
		printf("at uvs: %f, %f", uv.s, uv.t);
	}
#endif // DEBUG



	float r = (float)textureData[pixelIndex + 0]  / 255.0f;// * uv.x;
	float g = (float)textureData[pixelIndex + 1]  / 255.0f;// * uv.x;
	float b = (float)textureData[pixelIndex + 2]  / 255.0f;// * uv.x;

	// openGL applies gamma correction so we take it into account here and inverse-correct our texture space
	return vec3(pow(r, 2.2f), pow(g, 2.2f), pow(b, 2.2f)) * multiplier;
};

vec3 Texture2D::getColor(void* params_struct) {
	return vec3(0.0);
};