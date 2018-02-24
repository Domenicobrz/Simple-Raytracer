#pragma once

#include "common_math.h"
#include "Material.h"
#include "Primitive.h"
#include "Ray.h"


/* follows from: http://www.codinglabs.net/article_physically_based_rendering_cook_torrance.aspx */
class OrenNayarMaterial : public Material {
public:
	OrenNayarMaterial();
	OrenNayarMaterial(vec3);
	OrenNayarMaterial(vec3, float roughness);

	/* returns a color and changes the ray direction */
	vec3 compute(Primitive*, vec3, Ray&, vec2);


private:
	float roughness;
};