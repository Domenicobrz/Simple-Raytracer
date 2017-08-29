#include "AABB.h"

AABB::AABB() { };
AABB::AABB(vec3 c0, vec3 c1) : c0(c0), c1(c1) { };

AABB AABB::operator+(const AABB& b) const { 
	
	AABB newAABB;

	newAABB.c0.x = min(this->c0.x, b.c0.x);
	newAABB.c0.y = min(this->c0.y, b.c0.y);
	newAABB.c0.z = min(this->c0.z, b.c0.z);

	newAABB.c1.x = max(this->c1.x, b.c1.x);
	newAABB.c1.y = max(this->c1.y, b.c1.y);
	newAABB.c1.z = max(this->c1.z, b.c1.z);

	return newAABB;
}

AABB AABB::operator+(const vec3& v) const {

	AABB newAABB;

	newAABB.c0.x = min(this->c0.x, v.x);
	newAABB.c0.y = min(this->c0.y, v.y);
	newAABB.c0.z = min(this->c0.z, v.z);

	newAABB.c1.x = max(this->c1.x, v.x);
	newAABB.c1.y = max(this->c1.y, v.y);
	newAABB.c1.z = max(this->c1.z, v.z);

	return newAABB;
}

float AABB::intersect(Ray ray) {
	//float tmin = 0.000001f;
	float tmin = 0.0f;
	float tmax = INFINITY;
	
	// implement the intersection test from page 12 - RT:tnw
	for (int i = 0; i < 3; i++) {
		float invD = 1.f / ray.d[i];
		float t0 = (c0[i] - ray.o[i]) * invD;
		float t1 = (c1[i] - ray.o[i]) * invD;
		if (invD < 0.f) {  // swap t0 and t1
			float temp = t0; 
			t0 = t1; 
			t1 = temp;
		}

		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;

		if (tmax < tmin) return INFINITY;
	}

	return tmin;
}