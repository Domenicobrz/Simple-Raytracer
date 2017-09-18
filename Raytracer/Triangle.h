#pragma once

#include "Primitive.h"

class Triangle : public Primitive {
public:
	Triangle();
	Triangle(vec3, vec3, vec3);
	Triangle(vec3, vec3, vec3,
		     vec2, vec2, vec2);  // uvs

	void transform(mat4);

	float intersect(Ray);
	vec3  normalAtPoint(vec3);
	vec3  uvAtPoint(vec3);
	// returns interpolated texture uvs from supplied barycentric coordinates
	vec2  getInterpolatedUV(vec2);

	Material* getMaterial();
	AABB getAABB();

	vec3 getV0();
	vec3 getV1();
	vec3 getV2();

	vec2 getUV0();
	vec2 getUV1();
	vec2 getUV2();



	Material* material;


private:
	vec3 v0;
	vec3 v1;
	vec3 v2;
	vec2 uv0;
	vec2 uv1;
	vec2 uv2;
	vec3 normal;
	AABB boundingBox;
};