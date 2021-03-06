#pragma once

#include "common_math.h"
#include "Material.h"
#include "Primitive.h"
#include "Ray.h"

class GlossyMaterial : public Material {
public:
	GlossyMaterial();
	GlossyMaterial(vec3 color, float factor);

	vec3 compute(Primitive* prim, vec3 hitPoint, Ray& ray, vec2 uv);

private:
	float factor;
	//vec3 color;
};