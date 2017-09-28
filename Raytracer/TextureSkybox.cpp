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


	BYTE* textureBuffer = FreeImage_GetBits(texture);
	BYTE* endBuffer = textureBuffer + sideWidth * sideWidth * bytesPerPixel;
	//// vector here copies the data, it doesn't refer to the same data pointed by textureBuffer
	if (flagFace == TEXTURESKYBOX_posX) textureData_posX = std::vector<BYTE>(textureBuffer, endBuffer);
	if (flagFace == TEXTURESKYBOX_posY) textureData_posY = std::vector<BYTE>(textureBuffer, endBuffer);
	if (flagFace == TEXTURESKYBOX_posZ) textureData_posZ = std::vector<BYTE>(textureBuffer, endBuffer);
	if (flagFace == TEXTURESKYBOX_negX) textureData_negX = std::vector<BYTE>(textureBuffer, endBuffer);
	if (flagFace == TEXTURESKYBOX_negY) textureData_negY = std::vector<BYTE>(textureBuffer, endBuffer);
	if (flagFace == TEXTURESKYBOX_negZ) textureData_negZ = std::vector<BYTE>(textureBuffer, endBuffer);
	//textureData = std::vector<BYTE>(textureBuffer, textureBuffer + sideWidth * sideWidth * bytesPerPixel);


	if (texture) {
		// bitmap successfully loaded!
		FreeImage_Unload(texture);
	}
}

vec3 TextureSkybox::getColor(vec3 direction) {
	int majorAxis;
	float abs_x = abs(direction.x);
	float abs_y = abs(direction.y);
	float abs_z = abs(direction.z);

	/* pick the axis with the greatest absolute value as our face direction */
	if (abs_x > abs_y && abs_x > abs_z) majorAxis = 0;
	else if (abs_y > abs_z) majorAxis = 1;
	else majorAxis = 2;

	int u_axis = direction[sampleAxis[majorAxis][0]];
	int v_axis = direction[sampleAxis[majorAxis][1]];

	float u = ((-direction[u_axis] / abs(direction[majorAxis])) + 1.0f) / 2.0f;
	float v = ((-direction[v_axis] / abs(direction[majorAxis])) + 1.0f) / 2.0f;

#ifdef DEBUG
	if (u >= 1.0f || v >= 1.0f) {
		printf("uvs out of bounds: %d, %d", u, v);
		printf("From - TextureSkybox.getColor(vec3)");
	}
#endif // DEBUG


	//return vec3(pow(r, 2.2f), pow(g, 2.2f), pow(b, 2.2f)) * multiplier;
};
