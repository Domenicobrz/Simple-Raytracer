#include "Lambert.h"

LambertMaterial::LambertMaterial() : Material() { }; 
LambertMaterial::LambertMaterial(vec3 color) : Material(), color(color) { };

vec3 LambertMaterial::compute(Primitive* primitive, vec3 hitPoint, Ray& ray) {
	
	vec3 normal = primitive->normalAtPoint(hitPoint);
	if (normal.z < -0.99f) {
		int y = 0;
	}


	vec3 hitplusnormal = hitPoint + normal;

	vec3 randomPointOnUnitSphere;

	for (;;) {
		float x = rnd() * 2.0f - 1.0f;
		float y = rnd() * 2.0f - 1.0f;
		float z = rnd() * 2.0f - 1.0f;

		if (dot(vec3(x, y, z), vec3(x, y, z)) <= 1.0f) {
			randomPointOnUnitSphere = vec3(x, y, z);
			break;
		}
	}

	//vec3 randomPointOnUnitSphere = normalize(vec3(x, y, z)) * rnd();
	ray.o = hitPoint;
	ray.d = normalize((hitplusnormal + randomPointOnUnitSphere) - hitPoint);
	
	return color;
}