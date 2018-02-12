#pragma once

#include "common_math.h"
#include "Primitive.h"
#include "TextureManager.h"
#include "Ray.h"

class Material {
public:
	Material();
	Material(vec3 color);
	Material(vec3 color, vec3 emissive);
	virtual vec3 compute(Primitive* prim, vec3 hitPoint, Ray& ray, vec2 uv);
	virtual vec3 getColor(vec3 uvs, Primitive* prim);
	virtual vec3 getShininess(vec3 uvs, Primitive* prim);
	virtual vec3 getEmissive(Primitive* prim, vec3 hitPoint, Ray& ray, vec2 uv);
	vec3 HalfwayVector(vec3 wo, vec3 wi);

	vec3 color;
	vec3 emissive;
	TextureManager* tm; // treated as albedo
	TextureManager* specular; // could be used in Phong-BRDF 
	TextureManager* temissive; // could be used in Light-BRDF  -  be sure to override current implementation with tm if you choose this path

	bool procTextureFromPointPosition;
};