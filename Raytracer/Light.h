#pragma once

#include "common_math.h"
#include "Material.h"
#include "Primitive.h"
#include "Ray.h"
#include "TextureManager.h"

class LightMaterial : public Material {
public:
	LightMaterial();
	LightMaterial(vec3);

	/* returns a color and changes the ray direction */
	vec3 compute(Primitive*, vec3, Ray&, vec2 uv);
};