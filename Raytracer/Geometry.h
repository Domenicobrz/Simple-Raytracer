#pragma once

#include "common_math.h";
#include <vector>
#include "Primitive.h"
#include "Material.h"

class Geometry {
public:
	Geometry();

	/* scene will pass his vector of primitives here */
	virtual void concatGeometry(std::vector<Primitive*> &);
	virtual void setMaterial(Material*);
};