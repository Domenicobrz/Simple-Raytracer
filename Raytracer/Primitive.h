#pragma once

#include "Ray.h"
#include "AABB.h"

class Material;
class Primitive {
public:
	Primitive();

	virtual float intersect(Ray);
	virtual vec3  normalAtPoint(vec3);
	virtual Material* getMaterial();
	virtual AABB getAABB();

	virtual vec3 getV0();
	virtual vec3 getV1();
	virtual vec3 getV2();
};