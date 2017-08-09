#pragma once

#include "common_math.h"
#include "Primitive.h"
#include "Ray.h"


class Sphere : public Primitive {
public:
	Sphere();
	Sphere(vec3 center, float radius);

	float intersect(Ray);

	vec3 center;
	float radius;
};