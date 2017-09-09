#pragma once

#include "common_math.h"
#include "Material.h"
#include "Primitive.h"
#include "Ray.h"

class LambertMaterial : public Material {
public:
	LambertMaterial();
	LambertMaterial(vec3);

	/* returns a color and changes the ray direction */
	vec3 compute(Primitive*, vec3, Ray&);
	vec3 emissive();

	//vec3 color;
};