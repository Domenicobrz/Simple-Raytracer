#pragma once

#include "common_math.h"
#include "Primitive.h"
#include "Material.h"
#include "Ray.h"


class Sphere : public Primitive {
public:
	Sphere();
	Sphere(vec3 center, float radius);


	float intersect(Ray);
	vec3  normalAtPoint(vec3);
	Material* getMaterial();


	vec3 center;
	float radius;
	Material* material;
};