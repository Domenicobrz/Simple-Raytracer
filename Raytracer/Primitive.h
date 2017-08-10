#pragma once

#include "Ray.h"

class Material;
class Primitive {
public:
	Primitive();

	virtual float intersect(Ray);
	virtual vec3  normalAtPoint(vec3);
	virtual Material* getMaterial();
};