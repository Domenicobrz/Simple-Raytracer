#pragma once

#include "Ray.h"

class Primitive {
public:
	Primitive();

	virtual void intersect(Ray);
};