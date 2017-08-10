#include "Primitive.h"

Primitive::Primitive() {

}

float Primitive::intersect(Ray ray) {
	return 0;
};

vec3 Primitive::normalAtPoint(vec3 point) {
	return point;
};

Material* Primitive::getMaterial() {
	return nullptr;
};