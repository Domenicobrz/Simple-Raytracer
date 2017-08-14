#pragma once

#include "common_math.h"
#include "Ray.h"

class AABB {
public:
	AABB();
	AABB(vec3, vec3);

	float intersect(Ray);

	/* https://stackoverflow.com/questions/11161541/overloaded-addition-assignment-operator-in-c-for-two-more-than-two-objects */
	AABB operator+(const AABB&) const;
	AABB operator+(const vec3&) const;

	vec3 c0;
	vec3 c1;
};