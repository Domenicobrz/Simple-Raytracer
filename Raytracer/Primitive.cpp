#include "Primitive.h"

Primitive::Primitive() {

}

float Primitive::intersect(Ray ray) {
	return 0;
};

vec3 Primitive::normalAtPoint(vec3 point) {
	return point;
};

AABB Primitive::getAABB() { return AABB(); };

Material* Primitive::getMaterial() {
	return nullptr;
};