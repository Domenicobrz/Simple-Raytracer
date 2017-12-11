#pragma once

#include "common_math.h"
#include "Material.h"
#include "Primitive.h"
#include "Ray.h"


/* Scratch a Pixel Lambert BRDF - passes the furnace test */
class SAPLambertMaterial : public Material {
public:
	SAPLambertMaterial();
	SAPLambertMaterial(vec3);
	
	/* returns a color and changes the ray direction */
	vec3 compute(Primitive*, vec3, Ray&, vec2);

	//vec3 color;
};