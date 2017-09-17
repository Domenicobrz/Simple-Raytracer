#pragma once

#include "common_math.h"
#include "Primitive.h"
#include "TextureManager.h"
#include "Ray.h"

class Material {
public:
	Material();
	Material(vec3 color);
	Material(TextureManager* tmp);
	virtual vec3 compute(Primitive* prim, vec3 hitPoint, Ray& ray, vec2 uv);
	virtual vec3 getColor(vec3 uvs, Primitive* prim);
	virtual vec3 emissive(Primitive* prim, vec3 hitPoint, Ray& ray, vec2 uv);

	vec3 color;
	TextureManager* tm;
};