#pragma once

#include "common_math.h"
#include "Material.h"
#include "Primitive.h"
#include "Ray.h"


/* follows from: http://www.codinglabs.net/article_physically_based_rendering_cook_torrance.aspx */
class CookTorranceMaterial : public Material {
public:
	CookTorranceMaterial();
	CookTorranceMaterial(vec3);
	CookTorranceMaterial(vec3, float kd, float ks, float roughness);

	/* returns a color and changes the ray direction */
	vec3 compute(Primitive*, vec3, Ray&, vec2);
	float Distribution(vec3 n, vec3 h, float alpha);
	float Geometry(vec3 wo, vec3 wi, vec3 n, vec3 h);
	float Fresnel(vec3 wi, vec3 h, float costheta);
	vec3 FresnelColor(vec3 wi, vec3 h, float costheta);
	float partialFresnel(vec3 wi, vec3 h, float costheta, float n, float k);


private:
	float ks;
	float kd;
	float roughness;
};