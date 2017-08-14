#pragma once

#include "Primitive.h"

class Triangle : public Primitive {
public:
	Triangle();
	Triangle(vec3, vec3, vec3);

	float intersect(Ray);
	vec3  normalAtPoint(vec3);
	Material* getMaterial();
	AABB getAABB();



	Material* material;


private:
	vec3 v0;
	vec3 v1;
	vec3 v2;
	vec3 normal;
	AABB boundingBox;
};