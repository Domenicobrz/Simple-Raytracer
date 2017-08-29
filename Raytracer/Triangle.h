#pragma once

#include "Primitive.h"

class Triangle : public Primitive {
public:
	Triangle();
	Triangle(vec3, vec3, vec3);

	void transform(mat4);

	float intersect(Ray);
	vec3  normalAtPoint(vec3);
	Material* getMaterial();
	AABB getAABB();

	vec3 getV0();
	vec3 getV1();
	vec3 getV2();



	Material* material;


private:
	vec3 v0;
	vec3 v1;
	vec3 v2;
	vec3 normal;
	AABB boundingBox;
};