#pragma once

#include "Ray.h"

class Primitive {
public:
	Primitive();

	virtual float intersect(Ray);
};