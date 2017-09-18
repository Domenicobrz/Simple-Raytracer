#pragma once

#include "Ray.h"
#include "AABB.h"

class Material;
class Primitive {
public:
	Primitive();

	virtual float intersect(Ray);
	virtual vec3  normalAtPoint(vec3);
	virtual vec3  uvAtPoint(vec3);
	// returns the texture uvs from supplied barycentric coordinates
	virtual vec2  getInterpolatedUV(vec2);
	virtual Material* getMaterial();
	virtual AABB getAABB();

	virtual vec3 getV0();
	virtual vec3 getV1();
	virtual vec3 getV2();

	virtual vec2 getUV0();
	virtual vec2 getUV1();
	virtual vec2 getUV2();
};