#pragma once

#include "common_math.h"
#include "Material.h"
#include "Primitive.h"
#include "Ray.h"

class FogMaterial : public Material {
public:
	FogMaterial();
	FogMaterial(vec3);

	/* returns a color and changes the ray direction */
	vec3 compute(Primitive*, vec3, Ray&, vec2);
	bool refract(vec3 v, vec3 n, float ni_over_nt, vec3 & refracted);


	float refractionIndex = 1.4f;
};