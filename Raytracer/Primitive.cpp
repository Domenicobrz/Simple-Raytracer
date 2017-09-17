#include "Primitive.h"

Primitive::Primitive() {

}

float Primitive::intersect(Ray ray) {
	return 0;
};

vec3 Primitive::normalAtPoint(vec3 point) {
	return point;
};

vec3 Primitive::uvAtPoint(vec3 point) {
	return point;
};

AABB Primitive::getAABB() { return AABB(); };

Material* Primitive::getMaterial() {
	return nullptr;
};

vec3 Primitive::getV0() { return vec3(0); }
vec3 Primitive::getV1() { return vec3(0); }
vec3 Primitive::getV2() { return vec3(0); }