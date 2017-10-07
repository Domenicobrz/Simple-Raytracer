#pragma once

#include "common_math.h"
#include "Material.h"
#include "Primitive.h"
#include "Ray.h"

class FogMaterial : public Material {
public:
	FogMaterial();
	FogMaterial(vec3);
	FogMaterial(TextureManager*);

	/* returns a color and changes the ray direction */
	vec3 compute(Primitive*, vec3, Ray&, vec2);
	vec3 emissive(Primitive* prim, vec3 hitPoint, Ray& ray, vec2 uv);
	bool refract(vec3 v, vec3 n, float ni_over_nt, vec3 & refracted);


	float refractionIndex = 1.4f;
};