#include "TextureManager.h"

TextureManager::TextureManager() { };

vec3 TextureManager::getColor(vec3 coord) {
	//return vec3(0.0);
	return vec3(mod(coord.s * 0.2f, 1.0f), mod(coord.t * 0.2f, 1.0f), mod(coord.z * 0.2f, 1.0f));
};

vec3 TextureManager::getColor(void* params_struct) {
	return vec3(0.0);
};