#pragma once

#include "common_math.h"
#include "Material.h"
#include "Primitive.h"
#include "Ray.h"

class GlassMaterial : public Material {
public:
	GlassMaterial();
	GlassMaterial(vec3 color, float factor, float refractionIndex);
	vec3 compute(Primitive* prim, vec3 hitPoint, Ray& ray);
	vec3 emissive();

private:
	float factor;
	float refractionIndex;
	vec3 color;
};