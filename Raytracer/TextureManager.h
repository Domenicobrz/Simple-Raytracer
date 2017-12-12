#pragma once

#include "common_math.h"
#include "Primitive.h"

class TextureManager {
public:
	TextureManager();
	virtual vec3 getColor(vec3 uvq, Primitive* prim, bool gammaCorrect);
	// assume a particular texture requires a lot of additional params - we can tailor a material to pass fancy structs
	// to the getColor function as needed
	virtual vec3 getColor(void* params_struct);
};