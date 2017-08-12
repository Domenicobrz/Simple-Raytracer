#pragma once

#include "common_math.h"
#include "Primitive.h"
#include "Ray.h"

class Material {
public:
	Material();
	virtual vec3 compute(Primitive* prim, vec3 hitPoint, Ray& ray);
	virtual vec3 emissive();
};